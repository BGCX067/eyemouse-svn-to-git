#pragma once
#include "Utilitis.h"
#include "TrackingFuture.h"

#define EXTENDED_DESCRIPTOR 1

class CSURF
{
	CvMemStorage* m_Storage;
	CvSeq *objectKeypoints ,*objectDescriptors ;
	CvSURFParams m_Params ;
public:
	//===========================================================
	CSURF()
	{
		objectKeypoints  = objectDescriptors = NULL;
		m_Storage = cvCreateMemStorage(0);
		m_Params.hessianThreshold = 300;
	}

	//===========================================================
	void FindFutures(CvImage * frame , vector<CTrackingFuture>& TrackingFutures)
	{

		cvExtractSURF( frame, 0, &objectKeypoints, &objectDescriptors, m_Storage, m_Params );


		void findPairs(TrackingFutures,  objectKeypoints, objectDescriptors );


	}

//=====================================================================================


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
//=====================================================================================
int naiveNearestNeighbor( const float* vec, int laplacian,
                      const CvSeq* model_keypoints,
                      const CvSeq* model_descriptors )
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );

    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = compareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}
//=====================================================================================
void findPairs(vector<CTrackingFuture>   & TrackingFuture ,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors )
{
	vector<CTrackingFuture>::iterator it;
CvSeqReader reader, kreader;
  
    //cvStartReadSeq( model_descriptors, &reader, 0 );

    for( int i = 0; i < TrackingFuture.size(); i++ )
    {
		int nearest_neighbor = naiveNearestNeighbor( TrackingFuture[i].m_Descriptor,TrackingFuture[i].m_DescriptorPoint.laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
		{
			TrackingFuture[i].IsFind();
			cvStartReadSeq( imageKeypoints, &kreader, 0 );
			for(int j = 0 ; j < nearest_neighbor ; j++)
				 CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );

			const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;

			TrackingFuture[i].m_DescriptorPoint = *kp;
			TrackingFuture[i].m_Location = TrackingFuture[i].m_DescriptorPoint.pt;


		}
		else
		{
			CTrackingFuture tmp;
			tmp.m_Descriptor = 
			TrackingFuture.push_back(tmp);
			TrackingFuture[i].NoFind();
		}

    }
}
//=====================================================================================
};







//		
//#define CORRELATION_THRESHOLD 0.7
//// brute-force attempt at correlating the two sets of features
//void bruteMatch(CvMat **points1, CvMat **points2, CvSeq *kp1, CvSeq *desc1,
//CvSeq *kp2, CvSeq *desc2) {
//int i,j,k;
//double* avg1 = (double*)malloc(sizeof(double)*kp1->total);
//double* avg2 = (double*)malloc(sizeof(double)*kp2->total);
//double* dev1 = (double*)malloc(sizeof(double)*kp1->total);
//double* dev2 = (double*)malloc(sizeof(double)*kp2->total);
//int* best1 = (int*)malloc(sizeof(int)*kp1->total);
//int* best2 = (int*)malloc(sizeof(int)*kp2->total);
//double* best1corr = (double*)malloc(sizeof(double)*kp1->total);
//double* best2corr = (double*)malloc(sizeof(double)*kp2->total);
//float *seq1, *seq2;
//int descriptor_size = EXTENDED_DESCRIPTOR ? 128 : 64;
//for (i=0; i<kp1->total; i++) {
//// find average and standard deviation of each descriptor
//avg1[i] = 0;
//dev1[i] = 0;
//seq1 = (float*)cvGetSeqElem(desc1, i);
//for (k=0; k<descriptor_size; k++) avg1[i] += seq1[k];
//avg1[i] /= descriptor_size;
//for (k=0; k<descriptor_size; k++) dev1[i] +=
//(seq1[k]-avg1[i])*(seq1[k]-avg1[i]);
//dev1[i] = sqrt(dev1[i]/descriptor_size);
//
//// initialize best1 and best1corr
//best1[i] = -1;
//best1corr[i] = -1.;
//}
//for (j=0; j<kp2->total; j++) {
//// find average and standard deviation of each descriptor
//avg2[j] = 0;
//dev2[j] = 0;
//seq2 = (float*)cvGetSeqElem(desc2, j);
//for (k=0; k<descriptor_size; k++) avg2[j] += seq2[k];
//avg2[j] /= descriptor_size;
//for (k=0; k<descriptor_size; k++) dev2[j] +=
//(seq2[k]-avg2[j])*(seq2[k]-avg2[j]);
//dev2[j] = sqrt(dev2[j]/descriptor_size);
//
//// initialize best2 and best2corr
//best2[j] = -1;
//best2corr[j] = -1.;
//}
//double corr;
//for (i = 0; i < kp1->total; ++i) {
//seq1 = (float*)cvGetSeqElem(desc1, i);
//for (j = 0; j < kp2->total; ++j) {
//corr = 0;
//seq2 = (float*)cvGetSeqElem(desc2, j);
//for (k = 0; k < descriptor_size; ++k)
//corr += (seq1[k]-avg1[i])*(seq2[k]-avg2[j]);
//corr /= (descriptor_size-1)*dev1[i]*dev2[j];
//if (corr > best1corr[i]) {
//best1corr[i] = corr;
//best1[i] = j;
//}
//if (corr > best2corr[j]) {
//best2corr[j] = corr;
//best2[j] = i;
//}
//}
//}
//j = 0;
//for (i = 0; i < kp1->total; i++)
//if (best2[best1[i]] == i && best1corr[i] > CORRELATION_THRESHOLD)
//j++;
//if (j == 0) return; // no matches found
//*points1 = cvCreateMat(1,j,CV_32FC2);
//*points2 = cvCreateMat(1,j,CV_32FC2);
//CvPoint2D32f *p1, *p2;
//j = 0;
//for (i = 0; i < kp1->total; i++) {
//if (best2[best1[i]] == i && best1corr[i] > CORRELATION_THRESHOLD) {
//p1 = &((CvSURFPoint*)cvGetSeqElem(kp1,i))->pt;
//p2 = &((CvSURFPoint*)cvGetSeqElem(kp2,best1[i]))->pt;
//(*points1)->data.fl[j*2] = p1->x;
//(*points1)->data.fl[j*2+1] = p1->y;
//(*points2)->data.fl[j*2] = p2->x;
//(*points2)->data.fl[j*2+1] = p2->y;
//j++;
//}
//}
//free(best2corr);
//free(best1corr);
//free(best2);
//free(best1);
//free(avg1);
//free(avg2);
//free(dev1);
//free(dev2);
//}
//
//		//===========================================================
//	}
//




	////////////////////////////////////////////////////////////////////////
	class CTracker
	{
		CvRect m_ROI;
		CSURF m_FuturesLocalizator
	public:
		//==========================================================================
		CTracker(void)
		{


		}
		//==========================================================================
	public:
		~CTracker(void);

		int m_LerningFutureIneration; // iteration count to accept new futer	


		vector<CTrackingFuture> m_TrackingFutures;
		vector<CTrackingFuture> m_LossedTrackingFutures;
		vector<CTrackingFuture> m_NewTrackingFuturesCandidats;

		//==========================================================================
		void Update( CvImage * frame)	
		{

			m_FuturesLocalizator.FindFutures(frame,m_TrackingFutures)

		}

		//==========================================================================
		void Init( CvImage * frame )
		{



		}
		//==========================================================================
	};
