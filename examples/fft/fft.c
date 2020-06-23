#include <stdio.h>
#include <pthread.h>
#include "fft.h"
#ifdef SW
    #include <math.h>
#endif


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_data {
    int startidx;
    int maxidx;
};

void *fft(void *threadarg) {

    float cos_coefficients_table[512]={1.000000,0.999981,0.999925,0.999831,0.999699,0.999529,0.999322,0.999078,0.998795,0.998476,0.998118,0.997723,0.997290,0.996820,0.996313,0.995767,0.995185,0.994565,0.993907,0.993212,0.992480,0.991710,0.990903,0.990058,0.989177,0.988258,0.987301,0.986308,0.985278,0.984210,0.983105,0.981964,0.980785,0.979570,0.978317,0.977028,0.975702,0.974339,0.972940,0.971504,0.970031,0.968522,0.966976,0.965394,0.963776,0.962121,0.960431,0.958703,0.956940,0.955141,0.953306,0.951435,0.949528,0.947586,0.945607,0.943593,0.941544,0.939459,0.937339,0.935184,0.932993,0.930767,0.928506,0.926210,0.923880,0.921514,0.919114,0.916679,0.914210,0.911706,0.909168,0.906596,0.903989,0.901349,0.898674,0.895966,0.893224,0.890449,0.887640,0.884797,0.881921,0.879012,0.876070,0.873095,0.870087,0.867046,0.863973,0.860867,0.857729,0.854558,0.851355,0.848120,0.844854,0.841555,0.838225,0.834863,0.831470,0.828045,0.824589,0.821103,0.817585,0.814036,0.810457,0.806848,0.803208,0.799537,0.795837,0.792107,0.788346,0.784557,0.780737,0.776888,0.773010,0.769103,0.765167,0.761202,0.757209,0.753187,0.749136,0.745058,0.740951,0.736817,0.732654,0.728464,0.724247,0.720003,0.715731,0.711432,0.707107,0.702755,0.698376,0.693971,0.689541,0.685084,0.680601,0.676093,0.671559,0.667000,0.662416,0.657807,0.653173,0.648514,0.643832,0.639124,0.634393,0.629638,0.624859,0.620057,0.615232,0.610383,0.605511,0.600616,0.595699,0.590760,0.585798,0.580814,0.575808,0.570781,0.565732,0.560662,0.555570,0.550458,0.545325,0.540171,0.534998,0.529804,0.524590,0.519356,0.514103,0.508830,0.503538,0.498228,0.492898,0.487550,0.482184,0.476799,0.471397,0.465976,0.460539,0.455084,0.449611,0.444122,0.438616,0.433094,0.427555,0.422000,0.416430,0.410843,0.405241,0.399624,0.393992,0.388345,0.382683,0.377007,0.371317,0.365613,0.359895,0.354164,0.348419,0.342661,0.336890,0.331106,0.325310,0.319502,0.313682,0.307850,0.302006,0.296151,0.290285,0.284408,0.278520,0.272621,0.266713,0.260794,0.254866,0.248928,0.242980,0.237024,0.231058,0.225084,0.219101,0.213110,0.207111,0.201105,0.195090,0.189069,0.183040,0.177004,0.170962,0.164913,0.158858,0.152797,0.146730,0.140658,0.134581,0.128498,0.122411,0.116319,0.110222,0.104122,0.098017,0.091909,0.085797,0.079682,0.073565,0.067444,0.061321,0.055195,0.049068,0.042938,0.036807,0.030675,0.024541,0.018407,0.012272,0.006136,0.000000,-0.006136,-0.012272,-0.018407,-0.024541,-0.030675,-0.036807,-0.042938,-0.049068,-0.055195,-0.061321,-0.067444,-0.073565,-0.079682,-0.085797,-0.091909,-0.098017,-0.104122,-0.110222,-0.116319,-0.122411,-0.128498,-0.134581,-0.140658,-0.146730,-0.152797,-0.158858,-0.164913,-0.170962,-0.177004,-0.183040,-0.189069,-0.195090,-0.201105,-0.207111,-0.213110,-0.219101,-0.225084,-0.231058,-0.237024,-0.242980,-0.248928,-0.254866,-0.260794,-0.266713,-0.272621,-0.278520,-0.284408,-0.290285,-0.296151,-0.302006,-0.307850,-0.313682,-0.319502,-0.325310,-0.331106,-0.336890,-0.342661,-0.348419,-0.354164,-0.359895,-0.365613,-0.371317,-0.377007,-0.382683,-0.388345,-0.393992,-0.399624,-0.405241,-0.410843,-0.416430,-0.422000,-0.427555,-0.433094,-0.438616,-0.444122,-0.449611,-0.455084,-0.460539,-0.465976,-0.471397,-0.476799,-0.482184,-0.487550,-0.492898,-0.498228,-0.503538,-0.508830,-0.514103,-0.519356,-0.524590,-0.529804,-0.534998,-0.540171,-0.545325,-0.550458,-0.555570,-0.560662,-0.565732,-0.570781,-0.575808,-0.580814,-0.585798,-0.590760,-0.595699,-0.600616,-0.605511,-0.610383,-0.615232,-0.620057,-0.624859,-0.629638,-0.634393,-0.639124,-0.643832,-0.648514,-0.653173,-0.657807,-0.662416,-0.667000,-0.671559,-0.676093,-0.680601,-0.685084,-0.689541,-0.693971,-0.698376,-0.702755,-0.707107,-0.711432,-0.715731,-0.720003,-0.724247,-0.728464,-0.732654,-0.736817,-0.740951,-0.745058,-0.749136,-0.753187,-0.757209,-0.761202,-0.765167,-0.769103,-0.773010,-0.776888,-0.780737,-0.784557,-0.788346,-0.792107,-0.795837,-0.799537,-0.803208,-0.806848,-0.810457,-0.814036,-0.817585,-0.821103,-0.824589,-0.828045,-0.831470,-0.834863,-0.838225,-0.841555,-0.844854,-0.848120,-0.851355,-0.854558,-0.857729,-0.860867,-0.863973,-0.867046,-0.870087,-0.873095,-0.876070,-0.879012,-0.881921,-0.884797,-0.887640,-0.890449,-0.893224,-0.895966,-0.898674,-0.901349,-0.903989,-0.906596,-0.909168,-0.911706,-0.914210,-0.916679,-0.919114,-0.921514,-0.923880,-0.926210,-0.928506,-0.930767,-0.932993,-0.935184,-0.937339,-0.939459,-0.941544,-0.943593,-0.945607,-0.947586,-0.949528,-0.951435,-0.953306,-0.955141,-0.956940,-0.958703,-0.960431,-0.962121,-0.963776,-0.965394,-0.966976,-0.968522,-0.970031,-0.971504,-0.972940,-0.974339,-0.975702,-0.977028,-0.978317,-0.979570,-0.980785,-0.981964,-0.983105,-0.984210,-0.985278,-0.986308,-0.987301,-0.988258,-0.989177,-0.990058,-0.990903,-0.991710,-0.992480,-0.993212,-0.993907,-0.994565,-0.995185,-0.995767,-0.996313,-0.996820,-0.997290,-0.997723,-0.998118,-0.998476,-0.998795,-0.999078,-0.999322,-0.999529,-0.999699,-0.999831,-0.999925,-0.999981};
    float sin_coefficients_table[512]={0.000000,-0.006136,-0.012272,-0.018407,-0.024541,-0.030675,-0.036807,-0.042938,-0.049068,-0.055195,-0.061321,-0.067444,-0.073565,-0.079682,-0.085797,-0.091909,-0.098017,-0.104122,-0.110222,-0.116319,-0.122411,-0.128498,-0.134581,-0.140658,-0.146730,-0.152797,-0.158858,-0.164913,-0.170962,-0.177004,-0.183040,-0.189069,-0.195090,-0.201105,-0.207111,-0.213110,-0.219101,-0.225084,-0.231058,-0.237024,-0.242980,-0.248928,-0.254866,-0.260794,-0.266713,-0.272621,-0.278520,-0.284408,-0.290285,-0.296151,-0.302006,-0.307850,-0.313682,-0.319502,-0.325310,-0.331106,-0.336890,-0.342661,-0.348419,-0.354164,-0.359895,-0.365613,-0.371317,-0.377007,-0.382683,-0.388345,-0.393992,-0.399624,-0.405241,-0.410843,-0.416430,-0.422000,-0.427555,-0.433094,-0.438616,-0.444122,-0.449611,-0.455084,-0.460539,-0.465976,-0.471397,-0.476799,-0.482184,-0.487550,-0.492898,-0.498228,-0.503538,-0.508830,-0.514103,-0.519356,-0.524590,-0.529804,-0.534998,-0.540171,-0.545325,-0.550458,-0.555570,-0.560662,-0.565732,-0.570781,-0.575808,-0.580814,-0.585798,-0.590760,-0.595699,-0.600616,-0.605511,-0.610383,-0.615232,-0.620057,-0.624859,-0.629638,-0.634393,-0.639124,-0.643832,-0.648514,-0.653173,-0.657807,-0.662416,-0.667000,-0.671559,-0.676093,-0.680601,-0.685084,-0.689541,-0.693971,-0.698376,-0.702755,-0.707107,-0.711432,-0.715731,-0.720003,-0.724247,-0.728464,-0.732654,-0.736817,-0.740951,-0.745058,-0.749136,-0.753187,-0.757209,-0.761202,-0.765167,-0.769103,-0.773010,-0.776888,-0.780737,-0.784557,-0.788346,-0.792107,-0.795837,-0.799537,-0.803208,-0.806848,-0.810457,-0.814036,-0.817585,-0.821103,-0.824589,-0.828045,-0.831470,-0.834863,-0.838225,-0.841555,-0.844854,-0.848120,-0.851355,-0.854558,-0.857729,-0.860867,-0.863973,-0.867046,-0.870087,-0.873095,-0.876070,-0.879012,-0.881921,-0.884797,-0.887640,-0.890449,-0.893224,-0.895966,-0.898674,-0.901349,-0.903989,-0.906596,-0.909168,-0.911706,-0.914210,-0.916679,-0.919114,-0.921514,-0.923880,-0.926210,-0.928506,-0.930767,-0.932993,-0.935184,-0.937339,-0.939459,-0.941544,-0.943593,-0.945607,-0.947586,-0.949528,-0.951435,-0.953306,-0.955141,-0.956940,-0.958703,-0.960431,-0.962121,-0.963776,-0.965394,-0.966976,-0.968522,-0.970031,-0.971504,-0.972940,-0.974339,-0.975702,-0.977028,-0.978317,-0.979570,-0.980785,-0.981964,-0.983105,-0.984210,-0.985278,-0.986308,-0.987301,-0.988258,-0.989177,-0.990058,-0.990903,-0.991710,-0.992480,-0.993212,-0.993907,-0.994565,-0.995185,-0.995767,-0.996313,-0.996820,-0.997290,-0.997723,-0.998118,-0.998476,-0.998795,-0.999078,-0.999322,-0.999529,-0.999699,-0.999831,-0.999925,-0.999981,-1.000000,-0.999981,-0.999925,-0.999831,-0.999699,-0.999529,-0.999322,-0.999078,-0.998795,-0.998476,-0.998118,-0.997723,-0.997290,-0.996820,-0.996313,-0.995767,-0.995185,-0.994565,-0.993907,-0.993212,-0.992480,-0.991710,-0.990903,-0.990058,-0.989177,-0.988258,-0.987301,-0.986308,-0.985278,-0.984210,-0.983105,-0.981964,-0.980785,-0.979570,-0.978317,-0.977028,-0.975702,-0.974339,-0.972940,-0.971504,-0.970031,-0.968522,-0.966976,-0.965394,-0.963776,-0.962121,-0.960431,-0.958703,-0.956940,-0.955141,-0.953306,-0.951435,-0.949528,-0.947586,-0.945607,-0.943593,-0.941544,-0.939459,-0.937339,-0.935184,-0.932993,-0.930767,-0.928506,-0.926210,-0.923880,-0.921514,-0.919114,-0.916679,-0.914210,-0.911706,-0.909168,-0.906596,-0.903989,-0.901349,-0.898674,-0.895966,-0.893224,-0.890449,-0.887640,-0.884797,-0.881921,-0.879012,-0.876070,-0.873095,-0.870087,-0.867046,-0.863973,-0.860867,-0.857729,-0.854558,-0.851355,-0.848120,-0.844854,-0.841555,-0.838225,-0.834863,-0.831470,-0.828045,-0.824589,-0.821103,-0.817585,-0.814036,-0.810457,-0.806848,-0.803208,-0.799537,-0.795837,-0.792107,-0.788346,-0.784557,-0.780737,-0.776888,-0.773010,-0.769103,-0.765167,-0.761202,-0.757209,-0.753187,-0.749136,-0.745058,-0.740951,-0.736817,-0.732654,-0.728464,-0.724247,-0.720003,-0.715731,-0.711432,-0.707107,-0.702755,-0.698376,-0.693971,-0.689541,-0.685084,-0.680601,-0.676093,-0.671559,-0.667000,-0.662416,-0.657807,-0.653173,-0.648514,-0.643832,-0.639124,-0.634393,-0.629638,-0.624859,-0.620057,-0.615232,-0.610383,-0.605511,-0.600616,-0.595699,-0.590760,-0.585798,-0.580814,-0.575808,-0.570781,-0.565732,-0.560662,-0.555570,-0.550458,-0.545325,-0.540171,-0.534998,-0.529804,-0.524590,-0.519356,-0.514103,-0.508830,-0.503538,-0.498228,-0.492898,-0.487550,-0.482184,-0.476799,-0.471397,-0.465976,-0.460539,-0.455084,-0.449611,-0.444122,-0.438616,-0.433094,-0.427555,-0.422000,-0.416430,-0.410843,-0.405241,-0.399624,-0.393992,-0.388345,-0.382683,-0.377007,-0.371317,-0.365613,-0.359895,-0.354164,-0.348419,-0.342661,-0.336890,-0.331106,-0.325310,-0.319502,-0.313682,-0.307850,-0.302006,-0.296151,-0.290285,-0.284408,-0.278520,-0.272621,-0.266713,-0.260794,-0.254866,-0.248928,-0.242980,-0.237024,-0.231058,-0.225084,-0.219101,-0.213110,-0.207111,-0.201105,-0.195090,-0.189069,-0.183040,-0.177004,-0.170962,-0.164913,-0.158858,-0.152797,-0.146730,-0.140658,-0.134581,-0.128498,-0.122411,-0.116319,-0.110222,-0.104122,-0.098017,-0.091909,-0.085797,-0.079682,-0.073565,-0.067444,-0.061321,-0.055195,-0.049068,-0.042938,-0.036807,-0.030675,-0.024541,-0.018407,-0.012272,-0.006136};

    int p, num;
    struct thread_data *arg = (struct thread_data *)threadarg;
    int startidx = arg->startidx;
    int maxidx = arg->maxidx;

    float temp_R; // temporary storage complex variable
    float temp_I; // temporary storage complex variable
    int h, j, k;  // loop indexes
    int lower;    // Index of lower point in butterfly
    int step, stage, DFTpts;
    int numBF;   // Butterfly Width
    int N2 = 10; // N2=N>>1

    // bit reverse(X_R, X_I);
    step = N2;
    float a, e, c, s;

    for (p = startidx; p < maxidx; p++) {
        for (stage = 1; stage <= 10; stage++) {
            int DFTpts = 1 << stage;    // DFT = 2^stage = points in sub DFT
            int numBF = DFTpts / 2;     // Butterfly WIDTHS in sub-DFT
            float e = -6.283185307178 / DFTpts;
            // Perform butterflies for j-th stage
            butterfly_loop:
            for (int j = 0; j < numBF; j++) {
                // Compute butterflies that use same W**k
                    float c = cos_coefficients_table[j<<(10-stage)];
                    float s = sin_coefficients_table[j<<(10-stage)];
                  int i_lower = j + numBF; // index of lower point in butterfly
                  float temp_R = X_R[h][i_lower] * c - X_I[h][i_lower] * s;
                  float temp_I = X_I[h][i_lower] * c + X_R[h][i_lower] * s;
                  X_R[h][i_lower] = X_R[h][j] - temp_R;
                  X_I[h][i_lower] = X_I[h][j] - temp_I;
                  X_R[h][j] = X_R[h][j] + temp_R;
                  X_I[h][j] = X_I[h][j] + temp_I;

            }
        }
    }

    pthread_exit(NULL);
}

int main() {

    int i, j;
    int main_result = 0;
    // create the thread variables
    pthread_t threads[NUM_ACCEL];
    struct thread_data data[NUM_ACCEL];
/*
    for (int i = 0; i < ARRAY_ROW; i++){
        for (int j = 0; j < ARRAY_COLUMN; j++){
             X_R[i][j] = rand();
             X_I[i][j] = 0.0;
        }
    }
    std::cout << "X_R[][] = {" << std::endl;
    for (int i = 0; i < ARRAY_ROW; i++){
        std::cout << "{";
        for (int j = 0; j < ARRAY_COLUMN; j++){
            if (j == ARRAY_COLUMN-1)
                std::cout << X_R[i][j] << "}, " << std::endl;
            else
                std::cout << X_R[i][j] << ", ";
        }
    }
    std::cout << "};" << std::endl;
    std::cout << "X_I[][] = {" << std::endl;
    for (int i = 0; i < ARRAY_ROW; i++){
        std::cout << "{";
        for (int j = 0; j < ARRAY_COLUMN; j++){
            if (j == ARRAY_COLUMN-1)
                std::cout << X_I[i][j] << "}, " << std::endl;
            else
                std::cout << X_I[i][j] << ", ";
        }
    }
    std::cout << "};" << std::endl;
*/
    // initialize structs to pass into accels
    for (i = 0; i < NUM_ACCEL; i++) {
        data[i].startidx = i * OPS_PER_ACCEL;
        data[i].maxidx = (i + 1) * OPS_PER_ACCEL;
    }

    // fork the threads
    for (i = 0; i < NUM_ACCEL; i++) {
        pthread_create(&threads[i], NULL, fft, (void *)&data[i]);
    }

    // join the threads
    for (i = 0; i < NUM_ACCEL; i++) {
        pthread_join(threads[i], NULL);
    }
#ifdef SW
    float error = 0.0;
    float maxerror = 0.0;
    for (int i = 0; i < ARRAY_ROW; i++){
        for (int j = 0; j < ARRAY_COLUMN; j++){
            float newerror = fabs(X_R_gold[i][j]-X_R[i][j]) + fabs(X_I_gold[i][j]-X_I[i][j]);
            error += newerror;
              if(newerror > maxerror) {
                maxerror = newerror; 
                fprintf(stdout, "Max Error@%d: %f\n", i, maxerror);
              }
        }
    }

    if ((error/ARRAY_ROW/ARRAY_COLUMN) > .05 || maxerror > 2) { // This is somewhat arbitrary.  Should do proper error analysis.
      fprintf(stdout, "*******************************************\n");
      fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
      fprintf(stdout, "*******************************************\n");
      return 1;
    }
    else {
      fprintf(stdout, "*******************************************\n");
      fprintf(stdout, "PASS: The output matches the golden output!\n");
      fprintf(stdout, "*******************************************\n");
      return 0;
    }

#endif
    
    return 0;
}
