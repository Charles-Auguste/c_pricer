#include "Calibration.h"

// ExplicitModel
ExplicitModel::ExplicitModel():
	_S(100.),
	_r(0.05)
{}

ExplicitModel::ExplicitModel(const double& S, const double& r):
	_S(S),
	_r(r)
{}

ExplicitModel::ExplicitModel(const ExplicitModel& model):
	_S(model._S),
	_r(model._r)
{}

ExplicitModel& ExplicitModel::operator=(const ExplicitModel& model) {
	if (this != &model) {
		_S = model._S;
		_r = model._r;
	}
	return *this;
}

void ExplicitModel::SetS(const double& S) {
	_S = S;
}

void ExplicitModel::SetR(const double& r) {
	_r = r;
}

double ExplicitModel::GetS() const {
	return _S;
}

double ExplicitModel::GetR() const {
	return _r;
}


// ExplicitBlackScholesModel
ExplicitBlackScholesModel::ExplicitBlackScholesModel() :
	ExplicitModel(),
	_sigma(0.2)
{}

ExplicitBlackScholesModel::ExplicitBlackScholesModel(const double& S, const double& r,const double& sigma):
	ExplicitModel(S,r),
	_sigma(sigma)
{}

ExplicitBlackScholesModel::ExplicitBlackScholesModel(const ExplicitBlackScholesModel& model):
	ExplicitModel(model),
	_sigma(model._sigma)
{}

ExplicitBlackScholesModel& ExplicitBlackScholesModel::operator=(const ExplicitBlackScholesModel& model) {
	if (this != &model) {
		ExplicitModel::operator=(model);
		_sigma = model._sigma;
	}
	return *this;
}

ExplicitBlackScholesModel* ExplicitBlackScholesModel::clone() const {

	return new ExplicitBlackScholesModel(*this);

}

double ExplicitBlackScholesModel::GetSigma() const {
	return _sigma;
}

void ExplicitBlackScholesModel::SetSigma(const double& sigma) {

	_sigma = sigma;
}

double ExplicitBlackScholesModel::CallPrice(const double& K, const double& T) const {
	double d1 = (log(_S/K) + (_r + 0.5*_sigma*_sigma)*T)/ (_sigma * pow(T, 0.5));
	double d2 = d1 - _sigma * pow(T, 0.5);
	return _S * normalCDF(d1) - K * exp(-_r * T) * normalCDF(d2);
}

double ExplicitBlackScholesModel::Vega(const double& K, const double& T, const double& sigma_mkt) const {
	double d1 = (log(_S / K) + (_r + 0.5 * sigma_mkt * sigma_mkt) * T) / (sigma_mkt * pow(T, 0.5));
	return 0.5*_S*pow(T,0.5)*exp(-0.5*d1*d1)/pi;
}

double ExplicitBlackScholesModel::normalCDF(const double& x) const{
	return std::erfc(-x / std::sqrt(2)) / 2;
}


//ExplicitHestonModel
ExplicitHestonModel::ExplicitHestonModel():	
		ExplicitModel(),
		_kappa(1),
		_theta(0.2),
		_sigma(0.2),
		_rho(0),
		_v0(0.2)
{}

ExplicitHestonModel::ExplicitHestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, const double& S, const double& r):
	ExplicitModel(S,r),
	_kappa(kappa),
	_theta(theta),
	_sigma(sigma),
	_rho(rho),
	_v0(v0)
{};

ExplicitHestonModel::ExplicitHestonModel(const ExplicitHestonModel& other):
	ExplicitModel(other),
	_kappa(other._kappa),
	_theta(other._theta),
	_sigma(other._sigma),
	_rho(other._rho),
	_v0(other._v0)
{};

ExplicitHestonModel& ExplicitHestonModel::operator=(const ExplicitHestonModel& model){
	if (this != &model) {
		ExplicitModel::operator=(model);
		_kappa = model._kappa;
		_theta = model._theta;
		_sigma = model._sigma;
		_rho = model._rho;
		_v0 = model._v0;
	}
	return *this;
}

ExplicitHestonModel* ExplicitHestonModel::clone() const {
	return new ExplicitHestonModel(*this);
}

double ExplicitHestonModel::GetKappa() const {
	return _kappa;
}

double ExplicitHestonModel::GetTheta() const {
	return _theta;
}

double ExplicitHestonModel::GetSigma() const {
	return _sigma;
}

double ExplicitHestonModel::GetRho() const {
	return _rho;
}

double ExplicitHestonModel::GetV0() const  {
	return _v0;
}

void ExplicitHestonModel::SetKappa(const double& kappa) {
	_kappa=kappa;
}

void ExplicitHestonModel::SetTheta(const double& theta) {
	_theta = theta;
}

void ExplicitHestonModel::SetSigma(const double& sigma) {
	_sigma = sigma;
}

void ExplicitHestonModel::SetRho(const double& rho) {
	_rho = rho;
}

void ExplicitHestonModel::SetV0(const double& v0) {
	_v0 = v0;
}

Complex ExplicitHestonModel::C1(const double& T, const double& omega) const{
	double u = -1.0;
	Complex y = i * omega - 1.0;

	Complex a = _kappa - _rho * _sigma * y;
	Complex b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	Complex g = (a - b) / (a + b);

	Complex C = _r * i * omega * 0.5 * T * T + ((_kappa * _theta) / (_sigma * _sigma)) * ((a - b) * T - 2.0 * log(  ( 1.0-g*exp(-b*T) )   /  (1.0-g)  ));

	return C;
}

Complex ExplicitHestonModel::C2(const double& T, const double& omega) const {
	double u = 1.0;
	Complex y = i * omega;

	Complex a = _kappa - _rho * _sigma * y;
	Complex b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	Complex g = (a - b) / (a + b);

	Complex C = _r * i * omega * 0.5 * T * T + ((_kappa * _theta) / (_sigma * _sigma)) * ((a - b) * T - 2.0 * log((1.0 - g * exp(-b * T)) / (1.0 - g)));

	return C;
}

Complex ExplicitHestonModel::D1(const double& T, const double& omega) const {
	double u = -1.0;
	Complex y = i * omega - 1.0;

	Complex a = _kappa - _rho * _sigma * y;
	Complex b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	Complex g = (a - b) / (a + b);

	Complex D = ((a - b) / (_sigma * _sigma)) * ((1.0 - exp(-b * T)) / (1.0 - g * exp(-b * T)));

	return D;
}

Complex ExplicitHestonModel::D2(const double& T, const double& omega) const {

	double u = 1.0;
	Complex y = i * omega;

	Complex a = _kappa - _rho * _sigma * y;
	Complex b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	Complex g = (a - b) / (a + b);

	Complex D = ((a - b) / (_sigma * _sigma)) * ((1.0 - exp(-b * T)) / (1.0 - g * exp(-b * T)));

	return D;
}

Complex ExplicitHestonModel::phi1(const double& T, const double& omega) const {
	double x = log(_S);
	Complex phi = exp(C1(T, omega) + D1(T, omega) * _v0 + i * omega * x);
	return phi;
}

Complex ExplicitHestonModel::phi2(const double& T, const double& omega) const {
	double x = log(_S);
	Complex phi = exp(C2(T, omega) + D2(T, omega) * _v0 + i * omega*x);
	return phi;
}

Vector ExplicitHestonModel::gauss_hermite_points() const{
	//auto gauss_hermite_points_weights = boost::math::quadrature::gauss_hermite<double, degree>();
	//return gauss_hermite_points_weights.first;
    return { -10.15910925, -9.52090368, -8.992398, -8.52056928,
        -8.08518865, -7.67583994, -7.28627659, -6.91238153,
        -6.55125917, -6.20077356, -5.8592902, -5.52552109,
        -5.19842653, -4.87715008, -4.56097376, -4.24928644,
        -3.94156073, -3.63733588, -3.33620465, -3.03780334,
        -2.74180375, -2.4479069, -2.15583787, -1.86534153,
        -1.57617901, -1.28812467, -1.0009635, -0.71448878,
        -0.42850006, -0.14280124, 0.14280124, 0.42850006,
        0.71448878, 1.0009635, 1.28812467, 1.57617901,
        1.86534153, 2.15583787, 2.4479069, 2.74180375,
        3.03780334, 3.33620465, 3.63733588, 3.94156073,
        4.24928644, 4.56097376, 4.87715008, 5.19842653,
        5.52552109, 5.8592902, 6.20077356, 6.55125917,
        6.91238153, 7.28627659, 7.67583994, 8.08518865,
        8.52056928, 8.992398, 9.52090368, 10.15910925 };
}

Vector ExplicitHestonModel::gauss_hermite_weights() const{
    //auto gauss_hermite_points_weights = boost::math::quadrature::gauss_hermite<double, degree>();
    //return gauss_hermite_points_weights.second;
    return { 1.10958725e-45, 2.43974759e-40, 3.77162673e-36, 1.33255961e-32,
        1.71557315e-29, 1.02940600e-26, 3.34575696e-24, 6.51256726e-22,
        8.15364047e-20, 6.92324791e-18, 4.15244411e-16, 1.81662458e-14,
        5.94843052e-13, 1.48895735e-11, 2.89935901e-10, 4.45682278e-09,
        5.47555462e-08, 5.43351613e-07, 4.39428694e-06, 2.91874190e-05,
        1.60277335e-04, 7.31773557e-04, 2.79132483e-03, 8.93217836e-03,
        2.40612728e-02, 5.47189709e-02, 1.05298764e-01, 1.71776157e-01,
        2.37868905e-01, 2.79853118e-01, 2.79853118e-01, 2.37868905e-01,
        1.71776157e-01, 1.05298764e-01, 5.47189709e-02, 2.40612728e-02,
        8.93217836e-03, 2.79132483e-03, 7.31773557e-04, 1.60277335e-04,
        2.91874190e-05, 4.39428694e-06, 5.43351613e-07, 5.47555462e-08,
        4.45682278e-09, 2.89935901e-10, 1.48895735e-11, 5.94843052e-13,
        1.81662458e-14, 4.15244411e-16, 6.92324791e-18, 8.15364047e-20,
        6.51256726e-22, 3.34575696e-24, 1.02940600e-26, 1.71557315e-29,
        1.33255961e-32, 3.77162673e-36, 2.43974759e-40, 1.10958725e-45 } ;
}

Vector ExplicitHestonModel::P_1_2(const double& K, const double& T) const {
	double omega;
	double integrale1 = 0.;
	double integrale2 = 0.;
	double logK = log(K);
	Vector P(2,0.);
	Vector weights = gauss_hermite_weights();
	Vector points = gauss_hermite_points();

	int N = points.size();

	for (int k = 0; k < N; k++) {

		omega = points[k];
		integrale1 += weights[k] * ((phi1(T, omega) * exp((omega * omega) - i * omega * logK) / (i * omega))).real();
		integrale2 += weights[k] * ((phi2(T, omega) * exp((omega * omega) - i * omega * logK) / (i * omega))).real();


	}
	
	P[0] = 0.5 + (1 / (2 * pi)) * integrale1;
	P[1] = 0.5 + (1 / (2 * pi)) * integrale2;

	return P;
}

double ExplicitHestonModel::CallPrice(const double& K, const double& T) const {

	double P1 = P_1_2(K, T)[0];
	double P2 = P_1_2(K, T)[1];

	double price = _S * P1 - K * exp(-_r * T) * P2;

	double inf = max(_S - K * exp(-_r * T), 0.);
	double sup = _S;

	return max(inf,min(price,sup)); // On cap et floor avec les bornes d'arbitrage
}

double ExplicitHestonModel::PutPrice(const double& K, const double& T) const {
	double call = CallPrice(K,T);
	double price = call - (_S - K * exp(-_r * T));
	return price;
}


// OptimisationImpliedVolatility
OptimisationImpliedVolatility::OptimisationImpliedVolatility() :
	_epsilon_iv(0.005),
	_model_ptr(new ExplicitHestonModel),
	_bs_model_ptr(new ExplicitBlackScholesModel)
{}

OptimisationImpliedVolatility::OptimisationImpliedVolatility(const double& epsilon, const ExplicitHestonModel& model, const ExplicitBlackScholesModel& bs_model) :
	_epsilon_iv(epsilon),
	_model_ptr(model.clone()),
	_bs_model_ptr(bs_model.clone())

{};

OptimisationImpliedVolatility::OptimisationImpliedVolatility(const OptimisationImpliedVolatility& optim):
	_epsilon_iv(optim._epsilon_iv),
	_model_ptr(optim._model_ptr),
	_bs_model_ptr(optim._bs_model_ptr)
{};

ExplicitHestonModel* OptimisationImpliedVolatility::GetModelPtr() const {
	return _model_ptr;
}

ExplicitBlackScholesModel* OptimisationImpliedVolatility::GetBSModelPtr() const {
	return _bs_model_ptr;
}

double OptimisationImpliedVolatility::GetEpsilon() const {
	return _epsilon_iv;
}

void OptimisationImpliedVolatility::SetEpsilon(const double& epsilon) {
	_epsilon_iv = epsilon;
}

void OptimisationImpliedVolatility::implied_vol(const double& T, const double& K, const double& C) const {


	double a = 0.;
	double b = 3;

	double mid = 0.5*(a+b);
	_bs_model_ptr->SetSigma(mid);
	double BS = _bs_model_ptr->CallPrice(K, T);

	while (abs(C - BS) >= _epsilon_iv) {

		if (C > BS) {

			a = mid;
			mid = 0.5 * (a + b);
			_bs_model_ptr->SetSigma(mid);
			BS = _bs_model_ptr->CallPrice(K, T);
		}

		else {

			b = mid;
			mid = 0.5 * (a + b);
			_bs_model_ptr->SetSigma(mid);
			BS = _bs_model_ptr->CallPrice(K, T);
		}

	}

}

double OptimisationImpliedVolatility::loss_function(const std::vector<std::vector<double>>& IV_surface) const {

	// Le premier �l�ment du tableau est vide (ou zero)
	// La premi�re colonne contient les maturit�s (� partir de la deuxi�me ligne)
	// La premi�re ligne contient les strikes (� partie de la premi�re colonne)
	// Ainsi l'�lement � la i-�me ligne et j-�me colonne du tableau (i>=1 j>=1) 
	// contient la vol impli pour le prix C(K_j,T_i).

	double loss = 0.;
	int maturity_size = IV_surface.size();
	int strike_size = IV_surface[0].size();
	double T;
	double K;
	double call;
	double somme_poids=0.;

	for (int i = 1; i < maturity_size; i++) {

		T = IV_surface[i][0];
		for (int j = 1; j < strike_size; j++) {
			K = IV_surface[0][j];
			call = _model_ptr->CallPrice(K, T); // prix avec heston
			implied_vol(T, K, call);
            loss += pow(_bs_model_ptr->GetSigma() - IV_surface[i][j], 2) * _bs_model_ptr->Vega(K, T, IV_surface[i][j]);
			somme_poids += _bs_model_ptr->Vega(K, T, IV_surface[i][j]);

		}
		
	}
	return loss/somme_poids;


}

double OptimisationImpliedVolatility::loss_function_bis(const std::vector<std::vector<double>>& IV_surface) const {

	double loss = 0.;
	int maturity_size = IV_surface.size();
	int strike_size = IV_surface[0].size();
	double T;
	double K;
	double call_model;
	double call_market;


	for (int i = 1; i < maturity_size; i++) {

		T = IV_surface[i][0];
		for (int j = 1; j < strike_size; j++) {

			K = IV_surface[0][j];
			call_model = _model_ptr->CallPrice(K, T); // prix avec heston
			_bs_model_ptr->SetSigma(IV_surface[i][j]);
			call_market = _bs_model_ptr->CallPrice(K, T);

			
			loss += pow(call_model - call_market, 2); ///(_bs_model_ptr->Vega(K,T,IV_surface[i][j]));
		
		}

	}
	return loss;


}

void OptimisationImpliedVolatility::calibration(const std::vector<std::vector<double>>& IV_surface) const {

	//Utilisation descente de gradient

	const int N = 5; // Nombre de param�tres � calibrer (kappa, theta, sigma, rho, v0)
	const int nb_iter = 1000;
	const double dx = 1.0E-5;
	const double rate = 1.0E-8;
	const double tol = 1.0E-5; // Crit�re d'arret pour l'optim
	bool arret = true; //True si on continue l'optim et false sinon

	std::vector<double> gradient(N, 0.);
	double loss;
	double loss_dx;
	std::vector<double> params(N, 0.);


	// Initialisation du vecteur de param�tres

	params[0] = _model_ptr->GetKappa();
	params[1] = _model_ptr->GetTheta();
	params[2] = _model_ptr->GetSigma();
	params[3] = _model_ptr->GetRho();
	params[4] = _model_ptr->GetV0();


	int k = 0;


	while (k < nb_iter and arret) {

		//// Calcul du gradient 

		loss = loss_function(IV_surface);
		std::cout << "Fonction d'erreur :" << loss << std::endl;

		// Premi�re coordon�e


		_model_ptr->SetKappa(params[0] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetKappa(params[0]);

		gradient[0] = (loss_dx - loss) / dx;
		//std::cout << "gradient 0 " << gradient[0] << std::endl;



		// Deuxi�me 

		_model_ptr->SetTheta(params[1] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetTheta(params[1]);

		gradient[1] = (loss_dx - loss) / dx;


		// Trois�me 

		_model_ptr->SetSigma(params[2] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetSigma(params[2]);

		gradient[2] = (loss_dx - loss) / dx;


		// Quatri�me

		_model_ptr->SetRho(params[3] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetRho(params[3]);

		gradient[3] = (loss_dx - loss) / dx;


		// Cinqui�me

		_model_ptr->SetV0(params[4] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetV0(params[4]);

		gradient[4] = (loss_dx - loss) / dx;


		/// Mise � jour de params

		for (int i = 0; i < N; i++) {

			params[i] = params[i] - rate * gradient[i];


			if (i == 3) {

				params[i] = std::max(-1., std::min(params[i], 1.));

			}
			else {

				params[i] = std::max(0., params[i]);
			}


		}

		/// Mise � jour des attributs du mod�le

		_model_ptr->SetKappa(params[0]);
		_model_ptr->SetTheta(params[1]);
		_model_ptr->SetSigma(params[2]);
		_model_ptr->SetRho(params[3]);
		_model_ptr->SetV0(params[4]);


		// Incr�mentation de k
		k++;

		// Mise � jour du bool


		loss_dx = loss_function_bis(IV_surface);

		arret = abs(((loss - loss_dx) / loss)) > tol;


	}

}

void OptimisationImpliedVolatility::calibration_bis(const std::vector<std::vector<double>>& IV_surface) const {

	//Utilisation descente de gradient

	const int N = 5; // Nombre de param�tres � calibrer (kappa, theta, sigma, rho, v0)
	const int nb_iter = 5000; //Nombres it�rations maximum
	const double dx = 1.0E-3;
	const double rate = 1.0E-6; // pas d'apprentissage
	const double tol = 1.0E-10; // Crit�re d'arret pour l'optim
	bool arret = true; //True si on continue l'optim et false sinon

	std::vector<double> gradient(N, 0.);
	double loss;
	double loss_dx;
	std::vector<double> params(N, 0.);


	// Initialisation du vecteur de param�tres

	params[0] = _model_ptr->GetKappa();
	params[1] = _model_ptr->GetTheta();
	params[2] = _model_ptr->GetSigma();
	params[3] = _model_ptr->GetRho();
	params[4] = _model_ptr->GetV0();


	int k = 0;


	while (k < nb_iter and arret) {

		//// Calcul du gradient 

		loss = loss_function_bis(IV_surface);
		std::cout << "Fonction d'erreur :"<<loss << std::endl;

		// Premi�re coordon�e


		_model_ptr->SetKappa(params[0] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetKappa(params[0]);

		gradient[0] = (loss_dx - loss) / dx;
		//std::cout << "gradient 0 " << gradient[0] << std::endl;



		// Deuxi�me 

		_model_ptr->SetTheta(params[1] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetTheta(params[1]);

		gradient[1] = (loss_dx - loss) / dx;


		// Trois�me 

		_model_ptr->SetSigma(params[2] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetSigma(params[2]);

		gradient[2] = (loss_dx - loss) / dx;


		// Quatri�me

		_model_ptr->SetRho(params[3] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetRho(params[3]);

		gradient[3] = (loss_dx - loss) / dx;


		// Cinqui�me

		_model_ptr->SetV0(params[4] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetV0(params[4]);

		gradient[4] = (loss_dx - loss) / dx;


		/// Mise � jour de params

		for (int i = 0; i < N; i++) {

			params[i] = params[i] - rate * gradient[i];


			if (i == 3) {

				params[i] = std::max(-1., std::min(params[i], 1.));

			}
			else {

				params[i] = std::max(0., params[i]);
			}


		}



		/// Mise � jour des attributs du mod�le

		_model_ptr->SetKappa(params[0]);
		_model_ptr->SetTheta(params[1]);
		_model_ptr->SetSigma(params[2]);
		_model_ptr->SetRho(params[3]);
		_model_ptr->SetV0(params[4]);


		// Incr�mentation de k
		k++;

		// Mise � jour du bool


		loss_dx = loss_function_bis(IV_surface);

		arret = abs(((loss - loss_dx) / loss)) > tol;

		



	}

}

    
