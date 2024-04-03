#pragma once
#include <vector>
#include <complex>



class Model // pas une classe abstraite
{

public :

    // Constructeurs

    Model();
    Model(const double& S, const double& r);
    Model(const Model& model);

    // Destructeur

    virtual ~Model()=default;

    //Opérateur d'assignation

    Model& operator=(const Model& model);



protected:

    double _S; // prix initial de l'actif
    double _r; // taux court
};

class HestonModel : public Model
{

public :

    // Constructeurs
    HestonModel();
    HestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, const double& S, const double& r);
    HestonModel(const HestonModel& other);

    // Getteurs 

    double GetKappa() const;
    double GetTheta() const ;
    double GetSigma() const ;
    double GetRho() const ;
    double GetV0() const ;
    double GetS() const ;
    double GetR() const ;



    // Setteurs 

    void SetKappa(const double& kappa);
    void SetTheta(const double& theta);
    void SetSigma(const double& sigma);
    void SetRho(const double& rho);
    void SetV0(const double& v0);
    void SetS(const double& S);
    void SetR(const double& r);


    // Destructeurs

    ~HestonModel() override = default;

    // Opérateur d'assignation

    HestonModel& operator = (const HestonModel& model);

    // Méthode clone

    HestonModel* clone() const;

    // Méthodes à proprement à parler

    std::complex<double> C1(const double& T, const double& omega) const;
    std::complex<double> C2(const double& T, const double& omega) const ;

    std::complex<double> D1(const double& T, const double& omega) const ;
    std::complex<double> D2(const double& T, const double& omega) const ;

    std::complex<double> phi1(const double& T, const double& omega) const ;
    std::complex<double> phi2(const double& T, const double& omega) const ;
    
    std::vector<double> P_1_2(const double& K, const double& T) const ;
  
    
    double CallPrice(const double& K, const double& T) const ;
    double PutPrice(const double& K, const double& T) const ;



private :

    double _kappa;
    double _theta;
    double _sigma;
    double _rho;
    double _v0;

};



class BlackScholesModel : public Model
{

public :

    // Constructeurs
    BlackScholesModel();
    BlackScholesModel(const double& S, const double& r, const double& sigma);
    BlackScholesModel(const BlackScholesModel& model);

    // Destructeurs

    ~BlackScholesModel() override = default ;

    // Opérateur d'assignation

    BlackScholesModel& operator=(const BlackScholesModel& model);


    // Méthode clone

    BlackScholesModel* clone() const;

    // Getteurs

    double GetS() const;
    double GetR() const;
    double GetSigma() const;


    // Setteurs

    void SetS(const double& S);
    void SetR(const double& r);
    void SetSigma(const double& sigma);

    // Méthode de pricing

    double CallPrice(const double& K, const double& T) const;

    double Vega(const double&K, const double& T, const double& sigma_mkt) const;

private : 

   double _sigma;
};

