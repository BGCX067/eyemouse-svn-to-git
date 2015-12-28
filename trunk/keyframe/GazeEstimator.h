#pragma once
#include "utilitis.h"
#include "TrackingFuture.h"
class CPointTracker;

class CGazeEstimator
{
	bool m_bInitialized ;
	CvKalman* kalman ;
	CvMat* state ; /* (phi, delta_phi) */
	CvMat* process_noise ;
	CvMat* measurement ;
	CvRNG rng ;
	char code ;

	CvPoint2D32f m_LeftEyePositionMesure;
	CvPoint2D32f m_RightEyePositionMesure;

	CvPoint2D32f m_LeftEyePositionEstimate;
	CvPoint2D32f m_RightEyePositionEstimate;


	CvPoint2D32f m_PrevLeftEyePositionMesure;
public:
	//==============================================================================
	CGazeEstimator(void)
	{
		m_bInitialized = false;
		float A[] = { 1,0,  0,1};

		float H[] = { 1,0, 0,1};

		float B[] = { 1,0,   0,1};

		kalman = cvCreateKalman( 2, 2, 2 );
		process_noise = cvCreateMat( 2, 1, CV_32FC1 );
		measurement = cvCreateMat( 2, 1, CV_32FC1 );
		rng = cvRNG(-1);
		code = -1;
		//cvRandArr( &rng, state, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(0.1) );


		
		memcpy( kalman->transition_matrix->data.fl, A, sizeof(A));
		memcpy( kalman->measurement_matrix->data.fl, H, sizeof(H));
	//	memcpy( kalman->control_matrix->data.fl, H, sizeof(B));
		//kalman->measurement_matrix->data.fl[0] = 0;
		//kalman->measurement_matrix->data.fl[1] = 0;
	/*	kalman->measurement_matrix->data.fl[0] = 0;
		kalman->measurement_matrix->data.fl[1] = 0;*/
		cvSetIdentity( kalman->process_noise_cov, cvRealScalar(0.01) );
		cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(0.1) );

		//kalman->measurement_noise_cov->data.fl[0] = 0.9;
		//kalman->measurement_noise_cov->data.fl[1] = 0.9;

		//kalman->measurement_noise_cov->data.fl[2] = 0.9;
		//kalman->measurement_noise_cov->data.fl[3] = 0.9;
		cvSetIdentity( kalman->error_cov_post, cvRealScalar(0.1));

	//	memcpy( kalman->state_post->data.fl, H, sizeof(H));
	}

	//==============================================================================
	~CGazeEstimator(void)
	{

	}

	//==============================================================================
	CvPoint2D32f GetKonpensatePosition( CvPoint2D32f EyePositionMesure)
	{
		if (!m_bInitialized)
		{
			m_bInitialized = true;
			m_PrevLeftEyePositionMesure = EyePositionMesure;
	/*		kalman->measurement_matrix->data.fl[0] = EyePositionMesure.x;
			kalman->measurement_matrix->data.fl[1] = EyePositionMesure.y;*/
			kalman->state_post->data.fl[0] = EyePositionMesure.x;
			kalman->state_post->data.fl[1] = EyePositionMesure.y;
			kalman->state_post->data.fl[0] = 0;
			kalman->state_post->data.fl[1] = 0;
		};
		const CvMat* prediction = cvKalmanPredict(kalman, 0 );
		m_LeftEyePositionEstimate.x = prediction->data.fl[0];
		m_LeftEyePositionEstimate.y = prediction->data.fl[1];
		//float predict_v = prediction->data.fl[1];
		m_LeftEyePositionMesure = EyePositionMesure;
		float Vx =  m_PrevLeftEyePositionMesure.x - m_LeftEyePositionMesure.x;
		float Vy  = m_PrevLeftEyePositionMesure.y -  m_LeftEyePositionMesure.y;
		m_PrevLeftEyePositionMesure = m_LeftEyePositionEstimate;

		measurement->data.fl[0] = m_LeftEyePositionMesure.x;
		measurement->data.fl[1] = m_LeftEyePositionMesure.y;

		kalman->control_matrix->data.fl[0] = Vx;
		kalman->control_matrix->data.fl[0] = Vy;
		//measurement->data.fl[2] = Vx;
		//measurement->data.fl[3] =Vy;
		cvKalmanCorrect( kalman, measurement );

		return m_LeftEyePositionEstimate;

	}

	//===============================================================================

	///--------------kalman------------------------


};
