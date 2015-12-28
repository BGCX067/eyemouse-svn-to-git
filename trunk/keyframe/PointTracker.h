#pragma once
#include "TrackingFuture.h"
#include "Interfaces.h"
#include "TrackinkModel.h"



const int MaxFutureCount = 2000;

///////////////////////////////////////////////////////////////////////
class CPointTracker
{
public:
	friend class CTrackinkModel;

	shared_ptr<CTrackingFuture> m_CurrentModelFutures[MaxFutureCount];
	int m_InterationCount;


	int Counter;
	int m_FutureImgDescriptorSize ;
	int m_MinFutureDistans ;
	int m_FutureBlockSize;

	bool m_bFirstFrame;
	CvMemStorage* m_Storage;
	CvSURFParams m_Params; 

	//CvPoint2D32f m_InintHeadCenter;

	CvPoint2D32f* m_CurrentPoints;
	CvPoint2D32f* m_NewGoodPoints;
	CvPoint2D32f* m_NewPoints; 
	float  m_FitErrors[MaxFutureCount] ; 
	char  m_FindStatus[MaxFutureCount];
	int m_Flags;
	int m_OpticalFlowFindCount;

	CvSize m_WinSize;
	CvPoint m_SearchNewFutureWindowCenter;
	CvRect m_SearchNewFutureWindow;



	CvPoint2D32f m_PlotImgSizeFactor;
	IplImage *m_PlotResultImgGray;
	IplImage *m_PlotResultImg;
	IplImage *m_Frame;;
	IplImage *m_GreyImg , *m_PrevGreyImg , *m_PyramidImg , *m_PrevPyramidImg , *m_SwapTmpImg;
 	IplImage *m_PyramidImgKeyFrame , *m_PrevPyramidImgKeyFrame; 
	IplImage* m_EigImg ;
	IplImage* m_TmpImg;;

	IplImage *m_KeyFrameImg;
	IplImage *m_TransformKeyFrameImg;


	IplImage* m_pImgDescriptor ;


	float m_fScale;
	CvSize m_SearchNewFutureWindowSize;
	CTrackinkModel m_Model;

	CPointTracker(void)
	{
		m_LKFlag = 0;
		m_LKFlagKeyFrame = 0;
		m_Params = cvSURFParams(600, 1);
		m_AkceptFutureHesianThreshold = 790;
		m_Storage = cvCreateMemStorage(0);
		 M_fGoodToTrackQuality = 0.05;
	}

	double M_fGoodToTrackQuality ;

	int m_LKFlag;
		int m_LKFlagKeyFrame;
public:
	~CPointTracker(void)
	{
		cvReleaseMemStorage( &m_Storage );
	}

	bool m_InitFaze; 
	int m_AkceptFutureHesianThreshold;

	list<shared_ptr<CTrackingFuture>> m_vTrackingFutures;



	shared_ptr<CTrackingFuture> m_pLeftPupilCenter;
	shared_ptr<CTrackingFuture> m_pRightPupilCenter;

	
	shared_ptr<CTrackingFuture> m_pLeftEyeCornerL;
	shared_ptr<CTrackingFuture> m_pLeftEyeCornerR;
	shared_ptr<CTrackingFuture> m_pRightEyeCornerL;
	shared_ptr<CTrackingFuture> m_pRightEyeCornerR;


	//======================================================================================

	void Update(IplImage* frame,CvRect SearchNewFutureWindow);
	bool  CPointTracker::Initialize();
	void UpdateLK();
	bool FindFuter(CvPoint2D32f CurrentPoint );
	void ClearMissingFutures();
	bool IsGoodDistans(CvPoint2D32f pCurrentPoint);
	void  CPointTracker::Init(CvSize FrameSize, CvSize searchNewFutureWindowSize);
	void CPointTracker::TransformKeyFrame();
	void  CPointTracker::FindNewGoodFuterToTrack();
	void CPointTracker::OpticalFlow();
	void CPointTracker::EyeOpticalFlow();
	void CPointTracker::SortFutures();

	void CPointTracker::PlotResult();
	void CPointTracker::PlotPoint(CvPoint2D32f point);
	//===============================================================================
	void UpdateSurf(IplImage* frame)
	{

		CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
		cvExtractSURF( frame, 0, &objectKeypoints, &objectDescriptors, m_Storage, m_Params );
		findPairs( objectKeypoints, objectDescriptors );
	}
	//==================================================================

private:
	//======================================================================================

	double compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
	{
		double total_cost = 0;
		assert( length % 4 == 0 );
		for( int i = 0; i < length; i += 4 )
		{
			double t0 = d1[i] - d2[i];
			double t1 = d1[i+1] - d2[i+1];
			double t2 = d1[i+2] - d2[i+2];
			double t3 = d1[i+3] - d2[i+3];
			total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
			if( total_cost > best )
				break;
		}
		return total_cost;
	}

	//======================================================================================
	int naiveNearestNeighbor( const float* vec,const CvSURFPoint* pCurrentPoint,
		const CvSeq* model_keypoints,
		const CvSeq* model_descriptors ,  CvSURFPoint& newPoint  )
	{
		int length = (int)(model_descriptors->elem_size/sizeof(float));
		int i, neighbor = -1;
		double d, dist1 = 1e6, dist2 = 1e6;
		CvSeqReader reader, kreader;
		cvStartReadSeq( model_keypoints, &kreader, 0 );
		cvStartReadSeq( model_descriptors, &reader, 0 );

		for( i = 0; i < model_descriptors->total; i++ )
		{
			CvSURFPoint* kp = ( CvSURFPoint*)kreader.ptr;
			const float* mvec = (const float*)reader.ptr;
			CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
			CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
			/*		if( pCurrentPoint->laplacian != kp->laplacian )
			continue;*/

			if (abs(pCurrentPoint->size - kp->size) > 10 )
				continue;

			d = compareSURFDescriptors( vec, mvec, dist2, length );
			if( d < dist1 )
			{
				dist2 = dist1;
				dist1 = d;
				neighbor = i;
				newPoint = *kp;
			}
			else if ( d < dist2 )
				dist2 = d;
		}
		if ( dist1 < 0.6*dist2 )
			return neighbor;
		return -1;
	}

	//======================================================================================

	void findPairs(  const CvSeq* imageKeypoints, const CvSeq* imageDescriptors)
	{
		int i;
		CvSeqReader reader, kreader;

		list<shared_ptr<CTrackingFuture>>::iterator it ;

		CvSURFPoint newPoint; 


		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
			(*it)->m_IsLocated = false;

		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
		{
			(*it)->m_SURFPoint;

			CvSURFPoint CurrentPoint = (*it)->m_SURFPoint;
			float* CurrentDescriptor = (*it)->m_Descriptor;

			int nearest_neighbor = naiveNearestNeighbor( CurrentDescriptor, &CurrentPoint, imageKeypoints, imageDescriptors,newPoint );
			if( nearest_neighbor >= 0 )
			{
				(*it)->IsLocated();
				(*it)->m_SURFPoint.pt = newPoint.pt;
				(*it)->m_IsLocated = true;
				/*ptpairs.push_back(i);
				ptpairs.push_back(nearest_neighbor);*/
			}
			else	
			{
				(*it)->IsMissing();
			}
			// kasowanie dlugo nie odnajdywanych punktow
			//if ((*it)->m_FindCount  - (*it)->m_MissCount < - 10)
			//if((*it)->m_LastMissCount >40  && ((*it)->getScore() < 1))
			//{
			//	m_vTrackingFutures.erase(it);
			//	it = m_vTrackingFutures.begin();
			//}
		};

		////TODO: add new good futures to track

		CvSeqReader PointReader, DescriptorReader;
		cvStartReadSeq( imageKeypoints, &PointReader, 0 );
		cvStartReadSeq( imageDescriptors, &DescriptorReader, 0 );


		//bruteMatch(imageKeypoints, imageDescriptors);
		const CvSURFPoint* pCurrentPoint;

		bool GoodLocation = false;
		float distans = 0;
		for( int i = 0; i < imageKeypoints->total; i++ )
		{


			pCurrentPoint = (const CvSURFPoint*)PointReader.ptr;
			if (IsGoodDistans(pCurrentPoint->pt))
			{
				shared_ptr<CTrackingFuture> tmp(new CTrackingFuture() ) ;
				//CTrackingFuture * tmp(new CTrackingFuture() ) ;
				int size = imageDescriptors->elem_size;
				memcpy((char*)tmp->m_Descriptor,(char*)((float*)DescriptorReader.ptr),imageDescriptors->elem_size );

				//tmp->m_Descriptor = (const flat*)DescriptorReader.ptr;
				tmp->m_SURFPoint = *pCurrentPoint;	
				m_vTrackingFutures.push_back(tmp);
			};

			CV_NEXT_SEQ_ELEM(PointReader.seq->elem_size, PointReader );
			CV_NEXT_SEQ_ELEM( DescriptorReader.seq->elem_size, DescriptorReader );

		}

	}

	//======================================================================================






#define EXTENDED_DESCRIPTOR 0


#define CORRELATION_THRESHOLD 0.3
	// brute-force attempt at correlating the two sets of features
	void bruteMatch(const CvSeq *kp2,const  CvSeq *desc2)
	{

		list<shared_ptr<CTrackingFuture>>::iterator it ;

		/*			for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
		(*it)->m_IsLocated = false;*/



		int i,j,k;

		int 			FutureCount = m_vTrackingFutures.size();
		double* avg1 = (double*)malloc(sizeof(double)*FutureCount);
		double* avg2 = (double*)malloc(sizeof(double)*kp2->total);
		double* dev1 = (double*)malloc(sizeof(double)*FutureCount);
		double* dev2 = (double*)malloc(sizeof(double)*kp2->total);
		int* best1 = (int*)malloc(sizeof(int)*FutureCount);
		int* best2 = (int*)malloc(sizeof(int)*kp2->total);
		double* best1corr = (double*)malloc(sizeof(double)*FutureCount);
		double* best2corr = (double*)malloc(sizeof(double)*kp2->total);
		float *seq1, *seq2;
		int descriptor_size = EXTENDED_DESCRIPTOR ? 128 : 64;



		i = 0;
		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
		{

			(*it)->m_SURFPoint;
			avg1[i] = 0;
			dev1[i] = 0;
			seq1 = (float*)(*it)->m_Descriptor;
			for (k=0; k<descriptor_size; k++) avg1[i] += seq1[k];
			avg1[i] /= descriptor_size;
			for (k=0; k<descriptor_size; k++) dev1[i] +=
				(seq1[k]-avg1[i])*(seq1[k]-avg1[i]);
			dev1[i] = sqrt(dev1[i]/descriptor_size);

			// initialize best1 and best1corr
			best1[i] = -1;
			best1corr[i] = -1.;
			i++;
		};


		for (j=0; j<kp2->total; j++) {
			// find average and standard deviation of each descriptor
			avg2[j] = 0;
			dev2[j] = 0;
			seq2 = (float*)cvGetSeqElem(desc2, j);
			for (k=0; k<descriptor_size; k++) avg2[j] += seq2[k];
			avg2[j] /= descriptor_size;
			for (k=0; k<descriptor_size; k++) dev2[j] +=
				(seq2[k]-avg2[j])*(seq2[k]-avg2[j]);
			dev2[j] = sqrt(dev2[j]/descriptor_size);

			// initialize best2 and best2corr
			best2[j] = -1;
			best2corr[j] = -1.;
		}
		double corr;



		i = 0;
		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
		{

			seq1 = (float*)(*it)->m_Descriptor;



			//	for (i = 0; i < kp1->total; ++i) {
			//seq1 = (float*)cvGetSeqElem(desc1, i);
			for (j = 0; j < kp2->total; ++j) {
				corr = 0;
				seq2 = (float*)cvGetSeqElem(desc2, j);
				for (k = 0; k < descriptor_size; ++k)
					corr += (seq1[k]-avg1[i])*(seq2[k]-avg2[j]);
				corr /= (descriptor_size-1)*dev1[i]*dev2[j];
				if (corr > best1corr[i]) {
					best1corr[i] = corr;
					best1[i] = j;
				}
				if (corr > best2corr[j]) {
					best2corr[j] = corr;
					best2[j] = i;
				}
			};

			i++;
		}
		j = 0;


		for (i = 0; i < m_vTrackingFutures.size(); i++)
			if (best2[best1[i]] == i && best1corr[i] > CORRELATION_THRESHOLD)
				j++;
		if (j == 0) return; // no matches found



		/*CvPoint2D32f *p1, *p2;*/
		j = 0;
		i = 0;
		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
		{

			if (best2[best1[i]] == i && best1corr[i] > CORRELATION_THRESHOLD) {


				(*it)->IsLocated();
				(*it)->m_SURFPoint.pt =  ((CvSURFPoint*)cvGetSeqElem(kp2,best1[i]))->pt;
				(*it)->m_IsLocated = true;
			}
			else
			{
				(*it)->IsMissing();
			};

			i++;
		}
		free(best2corr);
		free(best1corr);
		free(best2);
		free(best1);
		free(avg1);
		free(avg2);
		free(dev1);
		free(dev2);

	}

	//===========================================================



};
