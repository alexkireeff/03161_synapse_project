// EPSP stands for "Exictatory Presynaptic Potential"
// Assumes one synapse per neuron
// Assumes EPSP will lead to vesicle fusion 100% of the time

// NMDAR active light
#define NMDAR_PIN 4

// Membrane Potential light
#define MP_PIN 5

// EPSP button pins
const int EPSP_PINS[] = {6, 7, 8};

// AMPAR light pins
const int AMPAR_PINS[] = {9, 10, 11};


// Constants
#define e 2.71828

// Model Constants
#define NUM_NEURONS 3

#define spike_thresh_mV -50
#define spike_refactory_period_seconds 1

#define membrane_max_mV 70
#define membrane_min_mV -70

#define nmdar_thresh_mV -60
#define nmdar_to_mV 0.5

// time constant for leaky neuron model
#define tau 5e-4

#define ampar_min_percentage 0
#define ampar_max_percentage 100
#define ampar_init_percentage 50
#define ampar_delta_percentage 1
#define ampar_percentage_to_mV 0.01


// keeps track of whether NMDAR can fire
bool nmdar_is_active;

// keeps track of membrane potential (milli Volts)
double membrane_potential_mV[NUM_NEURONS];

// keeps track of whether the epsp button was previously high
bool epsp_prev[NUM_NEURONS];

// Keeps tack of current state of epsp button
bool epsp_curr[NUM_NEURONS];

// keeps track of the current AMPAR count at each synapse (percentage)
int ampar_percent_saturation[NUM_NEURONS];



void setup() {
    pinMode(NMDAR_PIN, OUTPUT);
    nmdar_is_active = false;

    pinMode(MP_PIN, OUTPUT);

    for (int i = 0; i < NUM_NEURONS; i++){
        membrane_potential_mV[i] = (membrane_max_mV - membrane_min_mV) / NUM_NEURONS;

        pinMode(EPSP_PINS[i], INPUT);

        epsp_curr[i] = HIGH;

        epsp_prev[i] = HIGH;

        pinMode(AMPAR_PINS[i], OUTPUT);

        ampar_percent_saturation[i] = ampar_init_percentage;
    };

    Serial.begin(9600);
}



void loop() {
    // update membrane LED
    double aggregate_membrane_potential_mV = membrane_min_mV;
    for(int i = 0; i < NUM_NEURONS; i++){
        aggregate_membrane_potential_mV = aggregate_membrane_potential_mV + membrane_potential_mV[i];
    }

    // TODO comment me
    double standardized_aggregate_membrane_potential_mV = (aggregate_membrane_potential_mV - membrane_min_mV) / (membrane_max_mV - membrane_min_mV) * 255;

    // need to put in [0, 255] because that is the range of values for bytes without overflowing
    analogWrite(MP_PIN, byte(standardized_aggregate_membrane_potential_mV));


    // update NMDAR LED
    digitalWrite(NMDAR_PIN, nmdar_is_active);
    nmdar_is_active = nmdar_thresh_mV < aggregate_membrane_potential_mV;


    // for each synapse update the membrane potential
    for (int i = 0; i < NUM_NEURONS; i++){
        membrane_potential_mV[i] = membrane_potential_mV[i] / pow(e, tau);

        // read the current state of the button
        epsp_curr[i] = digitalRead(EPSP_PINS[i]);


        // write the current ampar count to the LED
        analogWrite(AMPAR_PINS[i], ampar_percent_saturation[i]);


        // update membrane potential if EPSP
        if (!epsp_prev[i] && epsp_curr[i]) {
            membrane_potential_mV[i] = membrane_potential_mV[i] + (ampar_percentage_to_mV * ampar_percent_saturation[i]);
            ampar_percent_saturation[i] = ampar_percent_saturation[i] - ampar_delta_percentage;

            membrane_potential_mV[i] = membrane_potential_mV[i] + nmdar_to_mV * nmdar_is_active;
        }


        // updated previous epsp to current epsp
        epsp_prev[i] = epsp_curr[i];

    }


    // handle spiking
    if (spike_thresh_mV < aggregate_membrane_potential_mV) {
        // TODO AMPAR growth

        // TODO spike threshold and then period where the voltage gated sodium channels can't fire again

    }


    // need to get new agg to normalize the membrane_potential_mV array
    double new_aggregate_membrane_potential_mV = membrane_min_mV;
    for(int i = 0; i < NUM_NEURONS; i++){
        new_aggregate_membrane_potential_mV += membrane_potential_mV[i];
    }


    // keep membrane potential and ampar count in model range
    for (int i = 0; i < NUM_NEURONS; i++){

        // NOTE assumes new_aggregate_membrane_potential_mV can't be 0 and out of range
        if (new_aggregate_membrane_potential_mV > membrane_max_mV){
            membrane_potential_mV[i] = membrane_potential_mV[i] / new_aggregate_membrane_potential_mV * membrane_max_mV;

        } else if (new_aggregate_membrane_potential_mV < membrane_min_mV) {
            membrane_potential_mV[i] = membrane_potential_mV[i] / new_aggregate_membrane_potential_mV * membrane_min_mV;

        }

        ampar_percent_saturation[i] = max(ampar_min_percentage, min(ampar_percent_saturation[i], ampar_max_percentage));
    }
}
