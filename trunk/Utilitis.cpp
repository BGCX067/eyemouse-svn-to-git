#include "Utilitis.h"

CTrackinkSystem* g_pTrackinkSystem;
//==========================================================================
CvPoint2D32f PointMul(CvPoint2D32f a, CvPoint2D32f b )
{
CvPoint2D32f result ;
result.x = a.x * b.x;
result.y = a.y * b.y;
return result;
}

//==========================================================================
CvPoint2D32f PointMul(CvPoint2D32f a, float b )
{
CvPoint2D32f result ;
result.x = a.x * b;
result.y = a.y * b;
return result;
}
//==========================================================================
CvPoint2D32f PointAdd(CvPoint2D32f a, CvPoint2D32f b )
{
CvPoint2D32f result ;
result.x = a.x + b.x;
result.y = a.y + b.y;
return result;
}
//==========================================================================
CvPoint2D32f PointSub(CvPoint2D32f a, CvPoint2D32f b )
{
CvPoint2D32f result ;
result.x = a.x - b.x;
result.y = a.y - b.y;
return result;
}



//==========================================================================
CvPoint3D32f PointSub(CvPoint3D32f a, CvPoint3D32f b )
{
CvPoint3D32f result ;
result.x = a.x - b.x;
result.y = a.y - b.y;
result.z = a.z- b.z;
return result;
}
//==========================================================================
double PointDistans(CvPoint2D32f a, CvPoint2D32f b )
{

	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));

}
//=========================================================================
bool PointInRect(CvPoint2D32f a, CvRect rect )
{
	return
(a.x > rect.x )&&(a.y > rect.y) &&
(a.x < rect.x + rect.width )&&(a.y < rect.y + rect.height) ;
}

//=========================================================================
//bool PointInElipse(CvPoint2D32f a,CvBox2D32f Elipse )
//{
//
//	return
//		(a.x > Elipse.center.x )&&(a.y > Elipse.center.y) &&
//(a.x < Elipse.center.x + Elipse.size.width )&&(a.y < Elipse.center.y + Elipse.size.height) ;
//}
//=========================================================================

bool PointInElipse(CvPoint2D32f a,CvBox2D32f Elipse, float margin )
{

	return
		(a.x > Elipse.center.x - Elipse.size.width * (0.5 + margin) )&&(a.y > Elipse.center.y- Elipse.size.height *  (0.5 + margin) ) &&
(a.x < Elipse.center.x + Elipse.size.width *( 0.5 +  margin ) )&&(a.y < Elipse.center.y + Elipse.size.height* (0.5 + margin)) ;
}
//=========================================================================