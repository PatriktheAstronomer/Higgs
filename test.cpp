// N-tuple Unit Test - made by Patrik Novotny, general physics at Charles University, in 2018 and 2019
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cstdlib>
#include <TRandom2.h>
#include <cmath>
#include <array>

const int events = 10000; //depends on number of variables
const int fault = 10; //count of wrong cases per variable - might be changed

const int iCount = 8; //count of IntVariables objects
const int uiCount = 9; //count of UnIntVariables objects
const int fCount = 8; //count of FloatVariables objects
const int tLorenzCount = 5;//count of TLorenzVariables objects
const int variablesCount = iCount + uiCount + fCount + tLorenzCount -1; //maximum count of related variables


TFile* file = TFile::Open( "unitTestFile.root", "recreate" ); // creates an output file
TTree* tree = new TTree( "NOMINAL", "" ); // creates TTree class which stores data
TRandom2 *r2 = new TRandom2();

struct IDstruct{
   array<int,variablesCount> type = {}; // types defined as 8 = int, 9 = unint, 10 = float, 11 =tlorenz
   array<int,variablesCount> ID = {};
};

class Variables{
	public:
		TString name;
		Float_t cutmax = 10000;
		Float_t cutmin = -10000;
		Float_t rightvalue;
		Float_t wrongvalue;
		Bool_t stability = 0;
		UInt_t cuttype;
        	IDstruct trigger;
	protected:
		Float_t getValue(){
            if (cuttype < 3){
                return r2->Uniform(cutmin,cutmax);
            }
            else{
                return rightvalue;
            }
        }
		Float_t getWrongValue(){
            if (cuttype == 1){
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
            else if(cuttype == 2 || cuttype == 0){
                if (cutmax > 0){
                    return r2->Uniform(cutmax,cutmax*2);
                }
                else if (cutmax == 0){
                    return (cutmin, -1);
                }
                else{
                   return r2->Uniform(cutmin,0);
                }
            }
            else if (cuttype >= 3){
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
		const Float_t value_2 = 0; //might be defined and used as values in future
		const Float_t value_3 = 0;
		const Float_t value_4 = 0;
		void SetRightValue (){
			value = getValue();
			FillVector();
		}
		void SetWrongValue (){
			value = getWrongValue();
			FillVector();
		}
		void FillVector(){
            pointer->SetPtEtaPhiM(value, value_2, value_3, value_4);
		}
};

enum{
    standard = 0,
    minimum,
    maximum,
    wrongright,
    rightonly,
    Int = 8,
    UnInt = 9,
    Float = 10,
    TLorentz = 11,
};


using namespace std;

void test() {

	IVariables IntVariable[iCount];
	UIVariables UnIntVariable[uiCount];
	FVariables FloatVariable[fCount];
    	TLorenzVariables TLorenzVariable[tLorenzCount];

    for(int k = 0; k < uiCount; k++){ //casem taky pujde odstranit
       		 UnIntVariable[k].cutmin = 0;
    }
	#include "cutsetup.h" //file with properties of each cut

    for(int k=0; k < variablesCount; k++){ //variable lze nahradit za nejvetsi count!!!...casem vymyslet pres pole s county (upotrebi se ve vsech 3 velkych loopech
        if (iCount >= k && IntVariable[k].cuttype == 4){
            IntVariable[k].wrongvalue = 5*IntVariable[k].rightvalue + 20; //Works for all data types
        }
	if (uiCount >= k &&UnIntVariable[k].cuttype == 4){
            UnIntVariable[k].wrongvalue = 5*UnIntVariable[k].rightvalue + 20;
        }
        if (fCount >= k && FloatVariable[k].cuttype == 4){
            FloatVariable[k].wrongvalue = 5*FloatVariable[k].rightvalue + 20.05;
        }
        if (tLorenzCount >= k && TLorenzVariable[k].cuttype == 4){
            TLorenzVariable[k].wrongvalue = 5*TLorenzVariable[k].rightvalue + 20.05;
        }
    }

    //branching for each data type ---prepsat dle horniho vzoru do jednoho cyklu
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
			if(IntVariable[i].stability == 0){
				IntVariable[i].SetRightValue();
                if(g<=i){
                    if(IntVariable[g].stability == 0){
                        IntVariable[g].SetWrongValue();
                    }
                }
			}
        }
        for(int u=0; u < uiCount; u++){
			if(UnIntVariable[u].stability == 0){
				UnIntVariable[u].SetRightValue();
                if(g<=u){
                   if(UnIntVariable[g].stability == 0){
                        UnIntVariable[g].SetWrongValue();
                    }
                }
			}
		}
        for(int f=0; f < fCount; f++){
			if(FloatVariable[f].stability == 0){
				FloatVariable[f].SetRightValue();
				if(g <= f){
                    if(FloatVariable[g].stability == 0){
                        FloatVariable[g].SetWrongValue();
                    }
                }
			}
		}
		for(int t=0; t < tLorenzCount; t++){
			if(TLorenzVariable[t].stability == 0){
				TLorenzVariable[t].SetRightValue();
				if(g <= t){
                    if(TLorenzVariable[g].stability == 0){
                        TLorenzVariable[g].SetWrongValue();
                    }
                    TLorenzVariable[g].FillVector();
                }
			}
			TLorenzVariable[t].FillVector();
		}

	for (int l = 0; l < variablesCount; l++){ //ORTerms check
                if (g <= iCount && IntVariable[l].trigger.type[0] != 0){ //in case it is not empty it cant be zero, first logical value is more uncommon

                    switch (IntVariable[g].trigger.type[l]){
                        case 8:
                            IntVariable[IntVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 9:
                            UnIntVariable[IntVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 10:
                            FloatVariable[IntVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 11:
                            TLorenzVariable[IntVariable[g].trigger.ID[l]].SetRightValue();
                            TLorenzVariable[IntVariable[g].trigger.ID[l]].FillVector();
                            break;
                    }
		}
		if (g <= uiCount && UnIntVariable[l].trigger.type[0] != 0){  
    		    switch (UnIntVariable[g].trigger.type[l]){
                        case 8:
                            IntVariable[UnIntVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 9:
                            UnIntVariable[UnIntVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 10:
                            FloatVariable[UnIntVariable[g].trigger.ID[l]].SetRightValue();
                            break;
			case 11:
                            TLorenzVariable[UnIntVariable[g].trigger.ID[l]].SetRightValue();
                            TLorenzVariable[UnIntVariable[g].trigger.ID[l]].FillVector();
                            break;
                    }
                }
                if(g <= fCount && FloatVariable[l].trigger.type[0] != 0){
                    switch (FloatVariable[g].trigger.type[l]){
                        case 8:
                            IntVariable[FloatVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 9:
                            UnIntVariable[FloatVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 10:
                            FloatVariable[FloatVariable[g].trigger.ID[l]].SetRightValue();
                            break;
                        case 11:
                            TLorenzVariable[FloatVariable[g].trigger.ID[l]].SetRightValue();
                            TLorenzVariable[FloatVariable[g].trigger.ID[l]].FillVector();
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
}//prepsat ify na slozene, voidy, pripsat tlorenzvector orcheck
