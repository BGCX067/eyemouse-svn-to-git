#pragma once
#include "utilitis.h"
#include "TrackingFuture.h"
class CPointTracker;

const float ErrorFactor = 20;
class CTrackinkModel
{
private:
	list<shared_ptr<CTrackingFuture>>::iterator it;
	double m_FundamentalMatrixData[9];
	CvMat m_FundamentalMatrix;
	CvMat* m_PerspectiveMatrix ;
	CvPoint2D32f* m_CurrentPoints;
	CvPoint2D32f* m_NewPoints; 

	CvPoint src_corners[4];
	CvPoint dst_corners[4];

	IplImage* m_OrgianlPerspectiveImg ;
	IplImage* m_PerspectiveImg;;


	///--------------kalman------------------------


	CvKalman* kalman ;
	CvMat* state ; /* (phi, delta_phi) */
	CvMat* process_noise ;
	CvMat* measurement ;
	CvRNG rng ;
	char code ;


public:
	CTrackinkModel(void);
public:

	CPointTracker* m_pPointTracker;

	CvPoint2D32f m_ModelCenter;

	CvPoint2D32f m_ModelCenterShiftAvr;

	CvPoint2D32f m_ModelCenter1;
	CvPoint2D32f m_ModelCenter2;

	CvPoint2D32f m_ModelCenterShift;
	CvPoint2D32f m_ModelShift;
	double m_ModelScale;

	~CTrackinkModel(void);

	void CTrackinkModel::CheckModelGemometry();
	void CTrackinkModel::UpdatePerspective();
	void CTrackinkModel::BuildModel();
	void CTrackinkModel::FindModelCenter();
	void CTrackinkModel::FindModelScale();
	void CTrackinkModel::FindModelCenterInternal();
	void CTrackinkModel::FindModelCenterShift();
	void  CTrackinkModel::Init(CPointTracker * Tracker);
	void  CTrackinkModel::InitPerspectiveImg();
	void  CTrackinkModel::DrawPerspecitve();

	
void  CTrackinkModel::Update();


};
