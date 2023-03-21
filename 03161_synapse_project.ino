// NOTE EPSP stands for "Exictatory Presynaptic Potential"

// NMDAR active LED
#define NMDAR_PIN 4

// Membrane Potential LED
#define MP_PIN 5

// EPSP button pins
const int EPSP_PINS[] = {6, 7, 8};

// AMPAR light pins
const int AMPAR_PINS[] = {9, 10, 11};

// Constants
#define e 2.71828

// Model Constants
#define NUM_NEURONS 3
#define nmdar_thresh 10
#define nmdar_epsp_const 10
#define tau 1e-4
#define ampar_epsp_const 1
#define ampar_DEC_const 10

// keep track if NMDAR should fire
bool nmdar_is_active;

// keep track of membrane potential
double membrane_potential[NUM_NEURONS]; // TODO -70 to 70 (mV)

// keeps track if the epsp button was previously high
bool epsp_prev[NUM_NEURONS];

// so we don't have to call digitalRead multiple times
bool epsp_curr[NUM_NEURONS];

// keeps track of the current AMPAR count at each synapse
int ampar_count[NUM_NEURONS]; // TODO 0 to 100 (percent)



void setup() {
    pinMode(NMDAR_PIN, OUTPUT);
    nmdar_is_active = false;

    pinMode(MP_PIN, OUTPUT);

    for (int i = 0; i < NUM_NEURONS; i++){
        membrane_potential[i] = 255;

        pinMode(EPSP_PINS[i], INPUT);

        epsp_curr[i] = HIGH;

        epsp_prev[i] = HIGH;

        pinMode(AMPAR_PINS[i], OUTPUT);
    };

    ampar_count[0] = 128;
    ampar_count[1] = 60;
    ampar_count[2] = 0;
}



void loop() {
    // TODO refactor code to have everything in correct units
    // TODO AMPAR growth
    // TODO spike threshold and then period where the voltage gated sodium channels can't fire again

    // update membrane LED
    int aggregate_membrane_potential = 0;

    for(int i = 0; i < NUM_NEURONS; i++){
        aggregate_membrane_potential += membrane_potential[i];
    }

    analogWrite(MP_PIN, byte(aggregate_membrane_potential));


    // update NMDAR LED
    digitalWrite(NMDAR_PIN, nmdar_is_active);
    nmdar_is_active = aggregate_membrane_potential > nmdar_thresh;


    for (int i = 0; i < NUM_NEURONS; i++){
        membrane_potential[i] = membrane_potential[i] / pow(e, tau);

        // read the current state of the button
        epsp_curr[i] = digitalRead(EPSP_PINS[i]);


        // write the current ampar count to the LED
        analogWrite(AMPAR_PINS[i], ampar_count[i]);


        // update membrane potential if EPSP
        if (!epsp_prev[i] && epsp_curr[i]) {
            membrane_potential[i] = membrane_potential[i] + (ampar_epsp_const * ampar_count[i]);
            ampar_count[i] = ampar_count[i] - ampar_DEC_const;

            if (nmdar_is_active) {
                membrane_potential[i] = membrane_potential[i] + nmdar_epsp_const;
            }
        }


        // updated previous epsp to current epsp
        epsp_prev[i] = epsp_curr[i];


        // keep numbers in range
        membrane_potential[i] = min(255, max(membrane_potential[i], 0));

        ampar_count[i] = min(255, max(ampar_count[i], 0));
    }
}
