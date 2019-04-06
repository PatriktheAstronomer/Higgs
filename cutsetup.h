IntVariable[0].name = "n_electrons"; // 1 is defined as stable, 0 as changeable
    IntVariable[0].value = 2;
    IntVariable[0].stability = 1;

    IntVariable[1].name = "n_muons";
    IntVariable[1].value = 0;
    IntVariable[1].stability = 1;

    IntVariable[2].name = "n_pvx";
    IntVariable[2].cutmin = 1;
    IntVariable[2].cuttype = minimum;

    IntVariable[3].name = "tau_0_id_medium";
    IntVariable[3].rightvalue = 1;
    IntVariable[3].cuttype = rightonly;

    IntVariable[4].name = "tau_1_id_medium";
    IntVariable[4].rightvalue = 1;
    IntVariable[4].cuttype = rightonly;

    IntVariable[5].name = "n_taus_medium";
    IntVariable[5].rightvalue = 0;
    IntVariable[5].cuttype = rightonly;

    IntVariable[6].name = "n_jets";
    IntVariable[6].cutmin = 1;
    IntVariable[6].cuttype = minimum;

    IntVariable[7].name = "n_bjets";
    IntVariable[7].rightvalue = 0;
    IntVariable[7].cuttype = rightonly;

    UnIntVariable[0].name = "tau_0";
    UnIntVariable[0].value = 2;
    UnIntVariable[0].stability = 1;

    UnIntVariable[1].name = "tau_1";
    UnIntVariable[1].value = 2;
    UnIntVariable[1].stability = 1;

    UnIntVariable[2].name = "tau_0_electron_trig_HLT_e60_lhmedium";
    UnIntVariable[2].rightvalue = 1;
    UnIntVariable[2].cuttype = rightonly;
    UnIntVariable[2].trigger.type[0] = UnInt;
    UnIntVariable[2].trigger.ID[0] = 3;
    UnIntVariable[2].trigger.type[1] = 9;
    UnIntVariable[2].trigger.ID[1] = 8;

    UnIntVariable[3].name = "tau_0_electron_trig_HLT_e120_lhloose";
    UnIntVariable[3].rightvalue = 1;
    UnIntVariable[3].cuttype = rightonly;
    UnIntVariable[3].trigger.type[0] = UnInt;
    UnIntVariable[3].trigger.ID[0] = 2;
    UnIntVariable[3].trigger.type[1] = UnInt;
    UnIntVariable[3].trigger.ID[1] = 8;

    UnIntVariable[4].name = "NOMINAL_pileup_random_run_number";
    UnIntVariable[4].rightvalue = 1;
    UnIntVariable[4].cuttype = rightonly;

    UnIntVariable[5].name = "event_clean_EC_LooseBad";
    UnIntVariable[5].rightvalue = 1;
    UnIntVariable[5].cuttype = rightonly;

    UnIntVariable[6].name = "tau_0_iso_Gradient";
    UnIntVariable[6].rightvalue = 1;
    UnIntVariable[6].cuttype = rightonly;

    UnIntVariable[7].name = "tau_1_iso_Gradient";
    UnIntVariable[7].rightvalue = 1;
    UnIntVariable[7].cuttype = rightonly;

    UnIntVariable[8].name = "tau_0_electron_trig_HLT_e24_lhmedium_L1EM20VH";
    UnIntVariable[8].rightvalue = 1;
    UnIntVariable[8].cuttype = rightonly;
    UnIntVariable[8].trigger.type[0] = UnInt;
    UnIntVariable[8].trigger.ID[0] = 2;
    UnIntVariable[8].trigger.type[1] = UnInt;
    UnIntVariable[8].trigger.ID[1] = 3;

    TLorenzVariable[0].name = "tau_0_p4";
    TLorenzVariable[0].value = 26.0;
    TLorenzVariable[0].stability = 1;

    TLorenzVariable[1].name = "tau_1_p4";
    TLorenzVariable[1].value = 16.0;
    TLorenzVariable[1].stability = 1;

    TLorenzVariable[2].name = "jet_0_p4";
    TLorenzVariable[2].cutmin = 40.0;
    TLorenzVariable[2].cuttype = minimum;

    TLorenzVariable[3].name = "met_p4";
    TLorenzVariable[3].cutmin = 55.0;
    TLorenzVariable[3].cuttype = minimum;

    TLorenzVariable[4].name = "met_hpto_p4";
    TLorenzVariable[4].cutmin = 55.0;
    TLorenzVariable[4].cuttype = minimum;

    FloatVariable[0].name = "tau_0_q";
    FloatVariable[0].cutmax = 0;
    FloatVariable[0].cuttype = maximum;

    FloatVariable[1].name = "tau_1_q";
    FloatVariable[1].value = 1;
    FloatVariable[1].stability = 1;

    FloatVariable[2].name = "ditau_deta";
    FloatVariable[2].cutmax = 1.5;
    FloatVariable[2].cuttype = maximum;

    FloatVariable[3].name = "ditau_dr";
    FloatVariable[3].cutmax = 2;
    FloatVariable[3].cuttype = maximum;

    FloatVariable[4].name = "ditau_coll_approx_m";
    FloatVariable[4].cutmin = 66.1876;
    FloatVariable[4].cuttype = minimum;

    FloatVariable[5].name = "ditau_coll_approx_x1";
    FloatVariable[5].cutmin = 0.1;
    FloatVariable[5].cutmax = 1;
    FloatVariable[5].cuttype = standard;

    FloatVariable[6].name = "ditau_coll_approx_x0";
    FloatVariable[6].cutmin = 0.1;
    FloatVariable[6].cutmax = 1;
    FloatVariable[6].cuttype = standard;

    FloatVariable[7].name = "ditau_vis_mass";
    FloatVariable[7].cutmin = 30.0;
    FloatVariable[7].cutmax = 75.0;
    FloatVariable[7].cuttype = standard;

