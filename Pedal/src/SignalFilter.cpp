#include "SignalFilter.h"





// v = s / t
// a = v/t
// a = s / t^2
// a = 300 / delta_t^2
// adjust model noise here s = 0.5 * a * delta_t^2 --> a = 2 * s / delta_t^2
//static const float KF_MODEL_NOISE_FORCE_ACCELERATION = ( 2.0f * 1000.0f / 0.05f/ 0.05f );
static const double KF_MODEL_NOISE_FORCE_ACCELERATION = ( 8.0f * 4.0f / 0.1f/ 0.1f );


float position = 0;        // Estimated position
float velocity = 0;        // Estimated velocity
float dt = 0.1f;            // Time step (seconds)

float P[2][2] = {          // Initial error covariance
    {0, 0},
    {0, 0}
};
float F[2][2] = {          // State transition matrix
    {1, dt},
    {0, 1}
};
float Q[2][2] = {          // Process noise covariance
    {0.1, 0.0},
    {0.0, 0.1}
};
float H[1][2] = {          // Measurement matrix
    {1, 0}
};
float R = 1;               // Measurement noise covariance
float K[2];                // Kalman Gain
float z;                   // Measurement (position)
float y;                   // Measurement residual
float S;                   // Residual covariance

void multiplyMatrices(float mat1[2][2], float mat2[2][2], float result[2][2]) {
    // Initialize the result matrix to zero
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i][j] = 0;
        }
    }
    
    // Perform matrix multiplication
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}



KalmanFilter::KalmanFilter(float varianceEstimate)
  : _timeLastObservation(micros())
{
  // initialize measurement error matrix
  R = varianceEstimate;
}

float KalmanFilter::filteredValue(float observation, float command, uint8_t modelNoiseScaling_u8) {


  // obtain time
  unsigned long currentTime = micros();
  unsigned long elapsedTime = currentTime - _timeLastObservation;
  float modelNoiseScaling_fl32 = modelNoiseScaling_u8;
  modelNoiseScaling_fl32 /= 255.0f;
  modelNoiseScaling_fl32 /= 1000.0f;
  modelNoiseScaling_fl32 /= 1000.0f;

  
  float modelNoiseLowerThreshold = 1e-9f; // 1/255 / 1000 / 1000 ca. 4*1e-9
  if (modelNoiseScaling_fl32 < modelNoiseLowerThreshold){ modelNoiseScaling_fl32 = modelNoiseLowerThreshold; }
  if (elapsedTime < 1) { elapsedTime=1; }
  if (elapsedTime > 5000) { elapsedTime=5000; }

  _timeLastObservation = currentTime;


  // update state transition and system covariance matrices
  //float delta_t = ((float)elapsedTime)  / 1000000.0f;/// 1000000.0f; // convert to seconds
  float delta_t = ((float)elapsedTime) / 1000.0f;
  float delta_t_pow2 = delta_t * delta_t;
  float delta_t_pow3 = delta_t_pow2 * delta_t;
  float delta_t_pow4 = delta_t_pow2 * delta_t_pow2;

  // update transition matrix
  // F = [1, T; 0, 1]
  F[0][0] = 1.0f;
  F[0][1] = delta_t;
  F[1][0] = 0.0f;
  F[1][1] = 1.0f;

  // Q = b * b' * a_var
  // b = [0.5 * T^2; T]
  // a_var: acceleration variance
  // Q = [0.25*T^4 , 0.5*T^3; 0.5*T^3, T^2
  float a_var = modelNoiseScaling_fl32 * KF_MODEL_NOISE_FORCE_ACCELERATION;
  float K_Q_11 = a_var * 0.5f * delta_t_pow3;
  Q[0][0] = a_var * 0.25f * delta_t_pow4;
  Q[0][1] = K_Q_11;
  Q[1][0] = K_Q_11;
  Q[1][1] = a_var * delta_t_pow2;

  // Predict Step
  // x_pred = x + T*v
  // v_pred = v
  float x_pred[2] = {
    position + delta_t * velocity,
    velocity
  };

  // transpose of F
  float Ftrans[2][2] = 
  {
    { F[0][0], F[1][0] }, 
    { F[0][1], F[1][1] }
  };
    
  float FP[2][2];
  float FPFtrans[2][2];
  multiplyMatrices(F, P, FP);
  multiplyMatrices(FP, Ftrans, FPFtrans);
  float P_pred[2][2] = {
    { FPFtrans[0][0] + Q[0][0], FPFtrans[0][1] + Q[0][1] }, 
    { FPFtrans[1][0] + Q[1][0], FPFtrans[1][1] + Q[1][1] }
  };

  // Update Step
  z = observation;  // Measurement
    
  // y = z - H*x_pred
  y = z - x_pred[0]; // Residual

  // S = (H * P * H' + R)
  // S = P + R, since H = [1, 0]
  S = P_pred[0][0] + R; // Residual covariance

  if (fabsf(S) > 0.000001f)
  {
    // K = P_pred * H' * inv(S)
    // since S is 1x1 --> inv(S) = 1 / S
    // P_pred * H' = [P_pred[0][0]; P_pred[1][0]  ], since H' = [1;0]
    K[0] = P_pred[0][0] / S;  // Kalman Gain
    K[1] = P_pred[1][0] / S;

    // Update state estimate
    position = x_pred[0] + K[0] * y;
    velocity = x_pred[1] + K[1] * y;

    // Update error covariance
    // P = (I - K*H)*P_pred
    // K*H = [K[0], 0; K[1], 0]
    // p_arg = (I - K*H)
    float p_arg[2][2] = {
      { (1.0f - K[0]),    0.0f},
      {-K[1],          1.0f}
    };
      
    multiplyMatrices(p_arg, P_pred, P);
  }
  else
  {
    P[0][0] = 0.0f;
    P[0][1] = 0.0f;
    P[1][0] = 0.0f;
    P[1][1] = 0.0f;
  }

  
      
  return position;


  // // obtain time
  // unsigned long currentTime = micros();
  // unsigned long elapsedTime = currentTime - _timeLastObservation;
  // double modelNoiseScaling_fl32 = modelNoiseScaling_u8;
  // modelNoiseScaling_fl32 /= 255.0;

  // if (modelNoiseScaling_fl32 < 0.001)
  // {
  //   modelNoiseScaling_fl32 = 0.001;
  // }
  // if (elapsedTime < 1) { elapsedTime=1; }
  // _timeLastObservation = currentTime;

  // // update state transition and system covariance matrices
  // double delta_t = ((double)elapsedTime)  / 1000000.0f;/// 1000000.0f; // convert to seconds
  // double delta_t_pow2 = delta_t * delta_t;
  // double delta_t_pow3 = delta_t_pow2 * delta_t;
  // double delta_t_pow4 = delta_t_pow2 * delta_t_pow2;

  // _K.F = {(double)1.0,  delta_t, 
  //         0.0,  (double)1.0};

  // _K.B = {1.0, 
  //         0.0};

  // double K_Q_11 = modelNoiseScaling_fl32 * KF_MODEL_NOISE_FORCE_ACCELERATION * (double)0.5f * delta_t_pow3;
  // _K.Q = {modelNoiseScaling_fl32 * KF_MODEL_NOISE_FORCE_ACCELERATION * (double)0.25f * delta_t_pow4,   K_Q_11,
  //       K_Q_11, modelNoiseScaling_fl32 * KF_MODEL_NOISE_FORCE_ACCELERATION * delta_t_pow2};
        

  // // APPLY KALMAN FILTER
  // _K.update({observation}, {command});
  // return _K.x(0,0);
}

float KalmanFilter::changeVelocity() {
  // return _K.x(0,1) / 1.0f;
  return velocity * 1000.0f;
}
