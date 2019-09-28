// N-tuple Unit Test - made by Patrik Novotny, general physics at Charles University, in 2018 and 2019
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <TRandom2.h>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <iostream>

TRandom2 *r2 = new TRandom2();

const int events = 10000; //depends on number of variables
const int fault = 10; //count of wrong cases per variable - might be changed

const int maximumVariable = 50; //maximum number of objects of each type

struct IDstruct{
   array<int,100> type = {}; // types defined as 8 = int, 9 = unint, 10 = float, 11 =tlorenz
   array<int,100> ID = {}; //variablesCount allocated as 100
};

enum{
    standard = 0, //generates data in range
    minimum,  // has only lower bound
    maximum, // has ony upper bound
    wrongright, // two cases
    rightonly, // only right value
    Int = 8,
    UnInt = 9,
    Float = 10,
    TLorentz = 11,
    Pt,
    Eta,
    Phi,
    M,
};

class Variables{
	public:
		TString name;
		Float_t cutmax = 10000;
			Float_t cutmin = 0;
		Float_t rightvalue = 999; // init value for debugging
		Float_t wrongvalue = 888; // init value for debugging
		Bool_t instability = 1; // if == 0, then products only one predefined value
		Bool_t fault = 1; // if == 0, then products only right data
		UInt_t fourMomentumType = 0;
		UInt_t cuttype;
        	IDstruct trigger;
	protected:
		Float_t getValue(){
             		return (cuttype < wrongright) ? r2->Uniform(cutmin,cutmax) : rightvalue;
            	}
		Float_t getWrongValue(){
            if (cuttype == minimum){
                if (cutmin > 0){
                    if (cutmin < 1){
                        return 0;
                    }
                    return r2->Uniform(0,cutmin-1); //because of int values with cutmin def as 1
                }
                else{
                    return r2->Uniform(cutmin*2-1,cutmin-1); //because of zeros
                }
            }
            else if(cuttype == maximum || cuttype == standard){
                if (cutmax > 0){
                    return r2->Uniform(cutmax*2,cutmax*3);
                }
                else if (cutmax == 0){
                    return (cutmin, -1);
                }
                else{
                   return r2->Uniform(cutmin,0);
                }
            }
            else if (cuttype >= wrongright){
                return wrongvalue;
            }
        return 666; // 1 and 0 as errors values are too common in this code
        }
};

class IVariables: public Variables{
	public:
		Int_t value = 0;
		void SetRightValue (){
			value = round(getValue());
		}
		void SetWrongValue (){
			value = round(getWrongValue());
		}
		void CheckOrTerms(){
		}
};

class UIVariables: public Variables{
	public:
		UInt_t value = 0; 
		void SetRightValue (){
			value = round(getValue());
		}
		void SetWrongValue (){
			value = round(getWrongValue());
		}
};

class FVariables: public Variables{
	public:
		Float_t value = 0;
		void SetRightValue (){
			value = getValue();
		}
		void SetWrongValue (){
			value = getWrongValue();
		}
};

class TLorenzVariables: public Variables{
	public:
		TLorentzVector* pointer = new TLorentzVector();
		
		Float_t value = 0;
		Float_t value_Pt = 0;
		Float_t value_Eta = 0; //might be defined and used as value in the future
		Float_t value_Phi = 0;
		Float_t value_M = 0;
		void SetRightValue (){
			value = getValue();
			FillVector();
		}
		void SetWrongValue (){
			value = getWrongValue();
			FillVector();
		}
		void FillVector(){
			if (fourMomentumType == Pt) value_Pt = value;
			else if (fourMomentumType == Eta) value_Eta = value;
			else if (fourMomentumType == Phi) value_Phi = value;
			else if (fourMomentumType == M) value_M = value;
			else{
				cout << "Undefined one of TLorenzVector types for \n";
				exit(0);
			}
            		pointer->SetPtEtaPhiM(value_Pt, value_Eta, value_Phi, value_M);
		}
};

using namespace std;
void test() {
	vector<string> regions {"e1mu", "emu1", "emu2", "mue1", "mu1e", "mue2"};
		/*where 
	e1mu means e+mu decay (electron leading), single-e trigger
	emu1 means e+mu decay (electron leading), single-mu trigger
	emu2 means e+mu decay (electron leading), di-lepton trigger
	mue1 means mu+e decay (muon leading), single-e trigger
	mu1e means mu+e decay (muon leading), single-mu trigger
	mue2 means mu+e decay (muon leading), di-lepton trigger
	*/

	vector<TString> runningOver = {}; //vector of used variables
	vector<TString> runningOverUnique = {}; // vector of used variables only for each branch

	IVariables IntVariable[maximumVariable];
	UIVariables UnIntVariable[maximumVariable];
	FVariables FloatVariable[maximumVariable];
	TLorenzVariables TLorenzVariable[maximumVariable];
	// booking variables, which are same for all channel
        IntVariable[0].name = "n_electrons"; // 1 is defined as stable, 0 as changeable
	IntVariable[0].value = 1;
        IntVariable[0].instability = 0;
     
        IntVariable[1].name = "n_muons";
	IntVariable[1].value = 1;
        IntVariable[1].instability = 0;
          
	IntVariable[2].name = "n_pvx";
    	IntVariable[2].cutmin = 1;
    	IntVariable[2].cuttype = minimum;
        
	IntVariable[3].name = "tau_0_id_medium";
	IntVariable[3].rightvalue = 1;
	IntVariable[3].wrongvalue = 0;
    	IntVariable[3].cuttype = wrongright;

	IntVariable[4].name = "tau_1_id_medium";
   	IntVariable[4].rightvalue = 1;
	IntVariable[4].wrongvalue = 0;
   	IntVariable[4].cuttype = wrongright;

	IntVariable[5].name = "n_taus_medium";
    	IntVariable[5].rightvalue = 0;
    	IntVariable[5].cuttype = rightonly;

   	IntVariable[6].name = "n_bjets";
   	IntVariable[6].rightvalue = 0;
   	IntVariable[6].cuttype = rightonly;

	IntVariable[7].name = "ditau_qxq";
        IntVariable[7].rightvalue = -1;
	IntVariable[7].wrongvalue = 1;
        IntVariable[7].cuttype = wrongright;

	IntVariable[8].name = "NOMINAL_pileup_random_run_number";
        IntVariable[8].cutmin = 266904;
	IntVariable[8].cutmax = 284484;
        IntVariable[8].cuttype = standard;

	UnIntVariable[0].name = "tau_0";
        UnIntVariable[0].instability = 0;

	UnIntVariable[1].name = "tau_1";
        UnIntVariable[1].instability = 0;

	UnIntVariable[2].name = "tau_0_electron_trig_HLT_e60_lhmedium";
        UnIntVariable[2].cuttype = wrongright;

	UnIntVariable[3].name = "tau_0_electron_trig_HLT_e120_lhloose";
        UnIntVariable[3].cuttype = wrongright;

	UnIntVariable[4].name = "empty_value";
	UnIntVariable[4].cuttype = standard;

	UnIntVariable[5].name = "tau_0_iso_Gradient";
	UnIntVariable[5].rightvalue = 1;
        UnIntVariable[5].wrongvalue = 0;
        UnIntVariable[5].cuttype = wrongright;

	UnIntVariable[6].name = "tau_1_iso_FCTightTrackOnly";
	UnIntVariable[6].rightvalue = 1;
        UnIntVariable[6].wrongvalue = 0;
        UnIntVariable[6].cuttype = wrongright;

        UnIntVariable[7].name = "n_taus_medium";
        UnIntVariable[7].rightvalue = 0;
        UnIntVariable[7].cuttype = rightonly;

	UnIntVariable[8].name = "tau_0_electron_trig_HLT_e24_lhmedium_L1EM20VH";
        UnIntVariable[8].cuttype = wrongright;

        UnIntVariable[9].name = "ditau_mmc_mlm_fit_status";
        UnIntVariable[9].rightvalue = 1;
	UnIntVariable[9].wrongvalue = 0;
        UnIntVariable[9].cuttype = wrongright;

        UnIntVariable[10].name = "tau_0_id_medium";
        UnIntVariable[10].rightvalue = 1;
	UnIntVariable[10].wrongvalue = 0;
        UnIntVariable[10].cuttype = wrongright;

        UnIntVariable[11].name = "tau_1_id_medium";
	UnIntVariable[11].rightvalue = 1;
	UnIntVariable[11].wrongvalue = 0;
        UnIntVariable[11].cuttype = wrongright;
	
	UnIntVariable[12].name = "tau_1_muon_trig_HLT_mu20_iloose_L1MU15";
        UnIntVariable[12].cuttype = wrongright;

        UnIntVariable[13].name = "tau_1_muon_trig_HLT_mu50";
        UnIntVariable[13].cuttype = wrongright;

	UnIntVariable[14].name = "tau_0_emu_trig_HLT_e17_lhloose_mu14";
        UnIntVariable[14].cuttype = wrongright;

        UnIntVariable[15].name = "tau_1_emu_trig_HLT_e17_lhloose_mu14";
        UnIntVariable[15].cuttype = wrongright;

	UnIntVariable[16].name = "tau_1_iso_Gradient";
	UnIntVariable[16].rightvalue = 1;
	UnIntVariable[16].wrongvalue = 0;
        UnIntVariable[16].cuttype = wrongright;

	UnIntVariable[17].name = "tau_0_iso_FCTightTrackOnly";
	UnIntVariable[17].rightvalue = 1;
	UnIntVariable[17].wrongvalue = 0;
        UnIntVariable[17].cuttype = wrongright;

	UnIntVariable[18].name = "tau_1_electron_trig_HLT_e60_lhmedium";
        UnIntVariable[18].cuttype = wrongright;

	UnIntVariable[19].name = "tau_1_electron_trig_HLT_e120_lhloose";
        UnIntVariable[19].cuttype = wrongright;

	UnIntVariable[20].name = "tau_1_electron_trig_HLT_e24_lhmedium_L1EM20VH";
        UnIntVariable[20].cuttype = wrongright;

	UnIntVariable[21].name = "tau_0_muon_trig_HLT_mu20_iloose_L1MU15";
        UnIntVariable[21].cuttype = wrongright;

        UnIntVariable[22].name = "tau_0_muon_trig_HLT_mu50";
        UnIntVariable[22].cuttype = wrongright;

	FloatVariable[0].name = "ditau_deta";
    	FloatVariable[0].cutmax = 1.5;
    	FloatVariable[0].cuttype = maximum;

    	FloatVariable[1].name = "ditau_dr";
    	FloatVariable[1].cutmax = 2;
    	FloatVariable[1].cuttype = maximum;

   	FloatVariable[2].name = "ditau_coll_approx_m";
   	FloatVariable[2].cutmin = 66.1876;
   	FloatVariable[2].cuttype = minimum;

	FloatVariable[3].name = "ditau_coll_approx_x1";
        FloatVariable[3].cutmin = 0.1;
        FloatVariable[3].cutmax = 1;
        FloatVariable[3].cuttype = standard;

        FloatVariable[4].name = "ditau_coll_approx_x0";
        FloatVariable[4].cutmin = 0.1;
        FloatVariable[4].cutmax = 1;
        FloatVariable[4].cuttype = standard;

	TLorenzVariable[0].name = "tau_0_p4";
	TLorenzVariable[0].cuttype = standard;
	TLorenzVariable[0].fourMomentumType = Pt;
        TLorenzVariable[0].fault = 0;
   
        TLorenzVariable[1].name = "tau_1_p4";
	TLorenzVariable[1].cuttype = standard;
	TLorenzVariable[1].fourMomentumType = Pt;
        TLorenzVariable[1].fault = 0;
       
        TLorenzVariable[2].name = "jet_0_p4";
	TLorenzVariable[2].cutmin = 40.0;
	TLorenzVariable[2].cuttype = minimum;
	TLorenzVariable[2].fourMomentumType = Pt;
       
        TLorenzVariable[3].name = "met_p4";
	TLorenzVariable[3].cutmin = 20.0;
        TLorenzVariable[3].cuttype = minimum;
	TLorenzVariable[3].fourMomentumType = Pt;

        TLorenzVariable[4].name = "ditau_p4";
	TLorenzVariable[4].cuttype = standard;
	TLorenzVariable[4].cutmin = 30;
	TLorenzVariable[4].cutmax = 100;
	TLorenzVariable[4].fourMomentumType = M;

    for (int i = 0; i < regions.size(); i++){
	int iCount = 0; //count of IntVariables objects
	int uiCount = 0; //count of UnIntVariables objects
	int fCount = 0; //count of FloatVariables objects
	int tLorenzCount = 0;//count of TLorenzVariables objects
	runningOver = {
			IntVariable[0].name,
			IntVariable[1].name,
			IntVariable[2].name,
			IntVariable[3].name,
			IntVariable[4].name,
			IntVariable[5].name,
			IntVariable[6].name,
			IntVariable[7].name,
			IntVariable[8].name,

			UnIntVariable[0].name,
			UnIntVariable[1].name,
			UnIntVariable[4].name,
			UnIntVariable[5].name,
			UnIntVariable[6].name,
			UnIntVariable[7].name,
			UnIntVariable[9].name,
			UnIntVariable[10].name,
			UnIntVariable[11].name,
			UnIntVariable[16].name,
			UnIntVariable[17].name,

			FloatVariable[0].name,
			FloatVariable[1].name,
			FloatVariable[2].name,
			FloatVariable[3].name,
			FloatVariable[4].name,

			TLorenzVariable[0].name,
			TLorenzVariable[1].name,
			TLorenzVariable[2].name,
			TLorenzVariable[3].name,
			TLorenzVariable[4].name,
		
		};

// filling part	
	switch (i){ // specification for each channel, which are not general are written bellow
		case (0): //e1mu
		runningOverUnique = {
			       UnIntVariable[2].name,
			       UnIntVariable[3].name,
			       UnIntVariable[8].name 
			       };

                UnIntVariable[0].value = 2; // tau0 type
                UnIntVariable[1].value = 1; // tau1 type
		UnIntVariable[5].rightvalue = 1; // tau_0_iso_Gradient
		UnIntVariable[5].wrongvalue = 0;
		UnIntVariable[6].rightvalue = 1; // tau_1_iso_FCTightTrackOnly
		UnIntVariable[6].wrongvalue = 0;
              
		UnIntVariable[2].rightvalue = 1; // tau_0_electron_trig_HLT_e60_lhmedium
		UnIntVariable[2].wrongvalue = 0;
                UnIntVariable[2].trigger.type[0] = UnInt;
                UnIntVariable[2].trigger.ID[0] = 3;
                UnIntVariable[2].trigger.type[1] = UnInt;
                UnIntVariable[2].trigger.ID[1] = 8;
              
                UnIntVariable[3].rightvalue = 1; // tau_0_electron_trig_HLT_e120_lhloos
		UnIntVariable[3].wrongvalue = 0;
                UnIntVariable[3].trigger.type[0] = UnInt;                
	       	UnIntVariable[3].trigger.ID[0] = 2;
                UnIntVariable[3].trigger.type[1] = UnInt;
                UnIntVariable[3].trigger.ID[1] = 8;
              
                UnIntVariable[8].rightvalue = 1; // tau_0_electron_trig_HLT_e24_lhmedium_L1EM20VH
		UnIntVariable[8].wrongvalue = 0;
                UnIntVariable[8].trigger.type[0] = UnInt;
                UnIntVariable[8].trigger.ID[0] = 2;
                UnIntVariable[8].trigger.type[1] = UnInt;
                UnIntVariable[8].trigger.ID[1] = 3;
              
 		TLorenzVariable[0].cutmax = 10000;
	        TLorenzVariable[0].cutmin = 25.0; // tau0 pt
      		break;

		case(1): //emu1
		runningOverUnique = {
			       UnIntVariable[12].name,
			       UnIntVariable[13].name,
			       };               
		UnIntVariable[0].value = 2; // tau0 type
                UnIntVariable[1].value = 1; // tau1 type
		UnIntVariable[5].rightvalue = 1; // tau_0_iso_Gradient
		UnIntVariable[5].wrongvalue = 0;
		UnIntVariable[6].rightvalue = 1; // tau_1_iso_FCTightTrackOnly
		UnIntVariable[6].wrongvalue = 0;

        	UnIntVariable[12].rightvalue = 1; //tau_1_muon_trig_HLT_mu20_iloose_L1MU15
		UnIntVariable[12].wrongvalue = 0;
		UnIntVariable[12].trigger.type[0] = UnInt;
		UnIntVariable[12].trigger.ID[0] = 13; 

        	UnIntVariable[13].rightvalue = 1; //tau_1_muon_trig_HLT_mu50
		UnIntVariable[13].wrongvalue = 0;
		UnIntVariable[13].trigger.type[0] = UnInt;
		UnIntVariable[13].trigger.ID[0] = 12; 

	        TLorenzVariable[0].cutmax = 25.0; // tau0 pt
		TLorenzVariable[0].cutmin = 0;
 
                TLorenzVariable[1].cutmin = 21.0; // tau1 pt
		TLorenzVariable[1].cutmax = 10000;
		break;
		case(2): //emu2
                runningOverUnique = {
			       UnIntVariable[14].name,
			       UnIntVariable[15].name,
			       };
		UnIntVariable[0].value = 2; // tau0 type
                UnIntVariable[1].value = 1; // tau1 type
		UnIntVariable[5].rightvalue = 1; // tau_0_iso_Gradient
		UnIntVariable[5].wrongvalue = 0;
		UnIntVariable[6].rightvalue = 1; // tau_1_iso_FCTightTrackOnly
		UnIntVariable[6].wrongvalue = 0;

		UnIntVariable[14].rightvalue = 1; // tau_0_emu_trig_HLT_e17_lhloose_mu14
		UnIntVariable[14].wrongvalue = 0;
		UnIntVariable[14].trigger.type[0] = UnInt;
		UnIntVariable[14].trigger.ID[0] = 15; 

        	UnIntVariable[15].rightvalue = 1; // tau_1_emu_trig_HLT_e17_lhloose_mu14
		UnIntVariable[16].wrongvalue = 0;
		UnIntVariable[15].trigger.type[0] = UnInt;
		UnIntVariable[15].trigger.ID[0] = 14; 

	        TLorenzVariable[0].cutmin = 18.0; 
	        TLorenzVariable[0].cutmax = 25.0; // tau0 pt

                TLorenzVariable[1].cutmin = 14.7; // tau1 pt
		TLorenzVariable[1].cutmax = 21.0;
		break;
		case(3): // mue1
		runningOverUnique = {
			       UnIntVariable[18].name,
			       UnIntVariable[19].name,
			       UnIntVariable[20].name 
			       };

		UnIntVariable[0].value = 1; // tau0 type
                UnIntVariable[1].value = 2; // tau1 type
		UnIntVariable[16].rightvalue = 1; // tau_1_iso_Gradient
		UnIntVariable[16].wrongvalue = 0;
		UnIntVariable[17].rightvalue = 1; // tau_0_iso_FCTightTrackOnly
		UnIntVariable[17].wrongvalue = 0;

		UnIntVariable[18].rightvalue = 1; // tau_1_electron_trig_HLT_e60_lhmedium
        	UnIntVariable[18].wrongvalue = 0;
		UnIntVariable[18].trigger.type[0] = UnInt;
                UnIntVariable[18].trigger.ID[0] = 19;
                UnIntVariable[18].trigger.type[1] = UnInt;
                UnIntVariable[18].trigger.ID[1] = 20;

		UnIntVariable[19].rightvalue = 1; // tau_1_electron_trig_HLT_e120_lhloose
	        UnIntVariable[19].wrongvalue = 0;
		UnIntVariable[19].trigger.type[0] = UnInt;
                UnIntVariable[19].trigger.ID[0] = 18;
                UnIntVariable[19].trigger.type[1] = UnInt;
                UnIntVariable[19].trigger.ID[1] = 20;

		UnIntVariable[20].rightvalue = 1; // tau_1_electron_trig_HLT_e24_lhmedium_L1EM20VH
        	UnIntVariable[20].wrongvalue = 0;
		UnIntVariable[20].trigger.type[0] = UnInt;
                UnIntVariable[20].trigger.ID[0] = 19;
                UnIntVariable[20].trigger.type[1] = UnInt;
                UnIntVariable[20].trigger.ID[1] = 18;

                TLorenzVariable[1].cutmin = 25.0; // tau1 pt
		TLorenzVariable[1].cutmax = 10000;
		break;
		case(4): // mu1e
		runningOverUnique = {
			       UnIntVariable[21].name,
			       UnIntVariable[22].name,
			       };
		UnIntVariable[0].value = 1; // tau0 type
                UnIntVariable[1].value = 2; // tau1 type
		UnIntVariable[16].rightvalue = 1; // tau_1_iso_Gradient
		UnIntVariable[16].wrongvalue = 0;
		UnIntVariable[17].rightvalue = 1; // tau_0_iso_FCTightTrackOnly
		UnIntVariable[17].wrongvalue = 0;

		UnIntVariable[21].rightvalue = 1; //tau_0_muon_trig_HLT_mu20_iloose_L1MU15
		UnIntVariable[21].wrongvalue = 0;
		UnIntVariable[21].trigger.type[0] = UnInt;
		UnIntVariable[21].trigger.ID[0] = 22; 

        	UnIntVariable[22].rightvalue = 1; //tau_0_muon_trig_HLT_mu50
		UnIntVariable[22].wrongvalue = 0;
		UnIntVariable[22].trigger.type[0] = UnInt;
		UnIntVariable[22].trigger.ID[0] = 21;

                TLorenzVariable[0].cutmin = 21.0; // tau0 pt
		TLorenzVariable[0].cutmax = 10000;

		TLorenzVariable[1].cutmax = 25.0; // tau1 pt
		TLorenzVariable[1].cutmin = 0;
		break;
		case(5): // mue2
                runningOverUnique = {
			       UnIntVariable[14].name,
			       UnIntVariable[15].name,
			       };
		UnIntVariable[0].value = 1; // tau0 type
                UnIntVariable[1].value = 2; // tau1 type
		UnIntVariable[16].rightvalue = 1; // tau_1_iso_Gradient
		UnIntVariable[16].wrongvalue = 0;
		UnIntVariable[17].rightvalue = 1; // tau_0_iso_FCTightTrackOnly
		UnIntVariable[17].wrongvalue = 0;

		UnIntVariable[14].rightvalue = 1; // tau_0_emu_trig_HLT_e17_lhloose_mu14
		UnIntVariable[14].wrongvalue = 0;
		UnIntVariable[14].trigger.type[0] = UnInt;
		UnIntVariable[14].trigger.ID[0] = 15; 

        	UnIntVariable[15].rightvalue = 1; // tau_1_emu_trig_HLT_e17_lhloose_mu14
		UnIntVariable[15].wrongvalue = 0;
		UnIntVariable[15].trigger.type[0] = UnInt;
		UnIntVariable[15].trigger.ID[0] = 14; 

                TLorenzVariable[0].cutmin = 14.7; // tau1 pt
		TLorenzVariable[0].cutmax = 21.0;

		TLorenzVariable[1].cutmin = 18.0; 
	        TLorenzVariable[1].cutmax = 25.0; // tau0 pt
		break;
  	}
   for (auto &str : runningOverUnique){
	runningOver.push_back(str);
   }

   TString outputFileName = regions.at(i)+"_unitTestFile.root";
   TFile* file = TFile::Open(outputFileName, "recreate" ); // creates an output file
   TTree* tree = new TTree( "NOMINAL", "" ); // creates TTree class which stores data
  // execution part
  	    
     for (int k = 0; k < maximumVariable; k++){ //define counts for all data types
  	 if (IntVariable[k].name != ""){
  		 iCount++;
  	 	}
  	   }
     for (int k = 0; k < maximumVariable; k++){
  	 if (FloatVariable[k].name != ""){
  		 fCount++;
  	 	}
  	   }
     for (int k = 0; k < maximumVariable; k++){
  	 if (UnIntVariable[k].name != ""){
  		 uiCount++;
  	 	}
  	   }
     for (int k = 0; k < maximumVariable; k++){
  	 if (TLorenzVariable[k].name != ""){
  		 tLorenzCount++;
  	 	}
  	   }
 
      const int variablesCount = iCount + uiCount + fCount + tLorenzCount -1; 
  
      for(int k = 0; k < uiCount; k++){
         	UnIntVariable[k].cutmin = 0;
      }
  
      for(int k=0; k < variablesCount; k++){
          if (iCount >= k && IntVariable[k].cuttype == rightonly){
              IntVariable[k].wrongvalue = 5*IntVariable[k].rightvalue + 20; //Works for all data types
          }
  	if (uiCount >= k &&UnIntVariable[k].cuttype == rightonly){
              UnIntVariable[k].wrongvalue =  0; //use this type for boolean variables
          }
          if (fCount >= k && FloatVariable[k].cuttype == rightonly){
              FloatVariable[k].wrongvalue = 5*FloatVariable[k].rightvalue + 20.05;
          }
          if (tLorenzCount >= k && TLorenzVariable[k].cuttype == rightonly){
              TLorenzVariable[k].wrongvalue = 5*TLorenzVariable[k].rightvalue + 20.05;
          }
      }
  
      //branching for each data type
  	for(int k = 0; k < iCount; k++){
  		tree->Branch(IntVariable[k].name, &IntVariable[k].value); //creating ntuple of ints
  	}
      	for(int k = 0; k < uiCount; k++){
  		tree->Branch(UnIntVariable[k].name, &UnIntVariable[k].value); //creating ntuple of ints
  	}
  	for(int k = 0; k < fCount; k++){
  		tree->Branch(FloatVariable[k].name, &FloatVariable[k].value); //creating ntuple of floats
  	}
      	for(int k = 0; k < tLorenzCount; k++){
  		tree->Branch(TLorenzVariable[k].name, &TLorenzVariable[k].pointer); //creating ntuple of booleans
  	}
 
      int g = 0;
      //filling values for each data type
  	for(int j=0; j < events; j++){
  		for(int i=0; i < iCount; i++){
  			if(IntVariable[i].instability){
  				IntVariable[i].SetRightValue();
                  		if(g<=iCount && (find(begin(runningOver), end(runningOver), IntVariable[g].name) != end(runningOver))){
                      			if(IntVariable[g].instability && IntVariable[g].fault){
                          			IntVariable[g].SetWrongValue();
                      			}
                  		}
  			}
          	}
          	for(int u=0; u < uiCount; u++){
  			if(UnIntVariable[u].instability){
  				UnIntVariable[u].SetRightValue();
                  		if(g-iCount<uiCount && g >= iCount  && (find(begin(runningOver), end(runningOver), UnIntVariable[g-iCount].name) != end(runningOver))){
                     			if(UnIntVariable[g-iCount].instability && UnIntVariable[g-iCount].fault){
                          			UnIntVariable[g-iCount].SetWrongValue();
                      			}
                  		}
  			}
  		}
          	for(int f=0; f < fCount; f++){
			if(FloatVariable[f].instability){
				FloatVariable[f].SetRightValue();
				if(g-iCount-uiCount < fCount && g >= iCount+uiCount && (find(begin(runningOver), end(runningOver), FloatVariable[g-iCount-uiCount].name) != end(runningOver))){
                    if(FloatVariable[g-iCount-uiCount].instability && FloatVariable[g-iCount-uiCount].fault){
                        FloatVariable[g-iCount-uiCount].SetWrongValue();
                    }
                }
			}
		}
		for(int t=0; t < tLorenzCount; t++){
			if(TLorenzVariable[t].instability){
				TLorenzVariable[t].SetRightValue();
				if(g-iCount-uiCount-fCount < tLorenzCount && g >= iCount+uiCount+fCount && (find(begin(runningOver), end(runningOver), TLorenzVariable[g-iCount-uiCount-fCount].name) != end(runningOver))){
                    if(TLorenzVariable[g-iCount-uiCount-fCount].instability && TLorenzVariable[g-iCount-uiCount-fCount].fault){
                        TLorenzVariable[g-iCount-uiCount-fCount].SetWrongValue();
                    }
                    TLorenzVariable[g-iCount-uiCount-fCount].FillVector();
                }
			}
			TLorenzVariable[t].FillVector();
		}
// check for conditions between variables
	for (int l = 0; l < variablesCount-1; l++){ //can not interact with self
                if (g <= iCount && IntVariable[g].trigger.type[0] != 0){

                    switch (IntVariable[g].trigger.type[l]){
			case 8:
                            IntVariable[IntVariable[g].trigger.ID[l]].SetWrongValue();
                            break;
                        case 9:
                            UnIntVariable[IntVariable[g].trigger.ID[l]].SetWrongValue();
                            break;
                        case 10:
                            FloatVariable[IntVariable[g].trigger.ID[l]].SetWrongValue();
                            break;
                        case 11:
                            TLorenzVariable[IntVariable[g].trigger.ID[l]].SetWrongValue();
                            TLorenzVariable[IntVariable[g].trigger.ID[l]].FillVector();
                            break;
                    }
		}
		if (g <= iCount+uiCount && g > iCount && UnIntVariable[g-iCount].trigger.type[0] != 0){	
    		    switch (UnIntVariable[g-iCount].trigger.type[l]){
                        case 8:
                            IntVariable[UnIntVariable[g-iCount].trigger.ID[l]].SetWrongValue();
                            break;
                        case 9:
                            UnIntVariable[UnIntVariable[g-iCount].trigger.ID[l]].SetWrongValue();
                            break;
                        case 10:
                            FloatVariable[UnIntVariable[g-iCount].trigger.ID[l]].SetWrongValue();
                            break;
			case 11:
                            TLorenzVariable[UnIntVariable[g-iCount].trigger.ID[l]].SetWrongValue();
                            TLorenzVariable[UnIntVariable[g-iCount].trigger.ID[l]].FillVector();
                            break;
                    }
                }
                if(g <= iCount+uiCount+fCount && g > iCount+uiCount && FloatVariable[g-iCount-uiCount].trigger.type[0] != 0){
                    switch (FloatVariable[g-iCount-uiCount].trigger.type[l]){
                        case 8:
                            IntVariable[FloatVariable[g-iCount-uiCount].trigger.ID[l]].SetWrongValue();
                            break;
                        case 9:
                            UnIntVariable[FloatVariable[g-iCount-uiCount].trigger.ID[l]].SetWrongValue();
                            break;
                        case 10:
                            FloatVariable[FloatVariable[g-iCount-uiCount].trigger.ID[l]].SetWrongValue();
                            break;
                        case 11:
                            TLorenzVariable[FloatVariable[g-iCount-uiCount].trigger.ID[l]].SetWrongValue();
			    TLorenzVariable[FloatVariable[g-iCount-uiCount].trigger.ID[l]].FillVector();
                            break;
                    }
                }
	}
		if (j%10 == 0){
            g++;
		}
		tree->Fill();
		}
	file->Write();
	file->Close();
	cout << "Channel " << regions.at(i) << " filled \n";
	}
}
