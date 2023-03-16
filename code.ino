// Membrane Potential is represented by the red LED at the top of the board
#define MP 5

// Synapses are the buttons
// Technically, these could be interrupts, but then I can only use 2
#define EPSP_1 6
#define EPSP_2 7
#define EPSP_3 8

// Number of AMPARs at each synapse
#define AMPAR_1 9
#define AMPAR_2 10
#define AMPAR_3 11



// Model Constants
#define tau 1e-4
#define e 2.71828
#define ampar_EPSP_const 1
#define ampar_DEC_const 10

// keep track of membrane potential
double membrane_potential;

// keeps track if the EPSP button was previously high
bool EPSP_prev_1;
bool EPSP_prev_2;
bool EPSP_prev_3;

// so we don't have to call digitalRead multiple times
bool EPSP_curr_1;
bool EPSP_curr_2;
bool EPSP_curr_3;

// keeps track of the current AMPAR count at each synapse
int ampar_1_count;
int ampar_2_count;
int ampar_3_count;



void setup() {
    pinMode(MP, OUTPUT);
    membrane_potential = 255;

    pinMode(EPSP_1, INPUT);
    pinMode(EPSP_2, INPUT);
    pinMode(EPSP_3, INPUT);

    EPSP_prev_1 = true;
    EPSP_prev_2 = true;
    EPSP_prev_3 = true;


    pinMode(AMPAR_1, OUTPUT);
    pinMode(AMPAR_2, OUTPUT);
    pinMode(AMPAR_3, OUTPUT);

    ampar_1_count = 128;
    ampar_2_count = 60;
    ampar_3_count = 0;
}



void loop() {
    // TODO NMDAR?
    // TODO spike threshold
    // TODO refactory period, until membrane_potential is 0
    // TODO AMPAR growth
    analogWrite(MP, byte(membrane_potential));
    membrane_potential = membrane_potential / pow(e, tau);

    EPSP_curr_1 = digitalRead(EPSP_1);
    EPSP_curr_2 = digitalRead(EPSP_2);
    EPSP_curr_3 = digitalRead(EPSP_3);

    analogWrite(AMPAR_1, ampar_1_count);
    analogWrite(AMPAR_2, ampar_2_count);
    analogWrite(AMPAR_3, ampar_3_count);

    if (!EPSP_prev_1 && EPSP_curr_1) {
        membrane_potential = membrane_potential + (ampar_EPSP_const * ampar_1_count);
        ampar_1_count = ampar_1_count - ampar_DEC_const;
    }

    if (!EPSP_prev_2 && EPSP_curr_2) {
        membrane_potential = membrane_potential + (ampar_EPSP_const * ampar_2_count);
        ampar_2_count = ampar_2_count - ampar_DEC_const;
    }

    if (!EPSP_prev_3 && EPSP_curr_3) {
        membrane_potential = membrane_potential + (ampar_EPSP_const * ampar_3_count);
        ampar_3_count = ampar_3_count - ampar_DEC_const;
    }

    // updated previous EPSP to current EPSP

    EPSP_prev_1 = EPSP_curr_1;
    EPSP_prev_2 = EPSP_curr_2;
    EPSP_prev_3 = EPSP_curr_3;


    // keep numbers in range

    membrane_potential = min(255, max(membrane_potential, 0));

    ampar_1_count = min(255, max(ampar_1_count, 0));
    ampar_2_count = min(255, max(ampar_2_count, 0));
    ampar_3_count = min(255, max(ampar_3_count, 0));


}
