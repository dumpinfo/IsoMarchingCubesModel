#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "c24bitmap.h"
#include "c256bitmap.h"
//#include "imgscale.h"
#include "VolRead.h"
#include "cisosurface.h"

short interpolate(unsigned short* PtPos,int x,int y,
float fu,float fv,int nScanWidth,int nScanHeight)
{
//相邻的四个像素最右下角点的x, y坐标偏移量
 int nx(1),ny(1);
 if((x + 1)>(nScanWidth  - 1)) nx = 0;
 if((y + 1)>(nScanHeight - 1)) ny = 0;
 short abyDot[2][2];//像素点(x, y)的数据位置
 unsigned  short* pbySrc = PtPos;//获取像素数值(x, y) = (x, y) + (0, 0)						
 abyDot[0][0] = *pbySrc;
 pbySrc = PtPos + nx ;//(x + 1, y) = (x, y) + (1, 0)
 abyDot[1][0] = *pbySrc;
 pbySrc = (PtPos - ny * nScanWidth);//指向下一行数据
 abyDot[0][1] = *pbySrc;//(x + 1, y + 1) = (x, y) + (1, 1)
 pbySrc = pbySrc+ nx;
 abyDot[1][1] = *pbySrc;
 short Val = (short)(BOUND(((1 - fu) * (1 - fv) * ((float)abyDot[0][0]) + 
				      (1 - fu) * fv * ((float)abyDot[0][1]) + 
					  fu * (1 - fv) * ((float)abyDot[1][0]) +
					  fu * fv * ((float)abyDot[1][1])), 0, 8000));
 return Val;
}

int main()
{
	CVolumeData VolDat1;
	VolDat1.FormatF(360,360,360);

	int i,j,t;
	int x,y,z;

	for(i = 0; i<360; i++)
	{
       for( j = 0; j <360; j++ )
		   for( t = 0; t < 360; t++ )
		   {
              *get_voxl_color(VolDat1,i,j,t) = 0;
		   }
	}
     
	int gridSize = 100;
	int gridThick =3;
	for(i = 20; i<340; i++)
	{
       for( j= 20; j < 340; j++ )

		   for( t= 20 ; t < 340; t++ )
		   {
			   //z = 40  + i;
			   //x = 128 + j;
			   //y = 128 + t;
			   *get_voxl_color(VolDat1,i,j,t) = 8000; 
		   }
	}

	for(i = 20; i<340; i++)
	{
       for( j= 30; j < 330; j++ )
		   for( t= 30 ; t < 330; t++ )
		   {
			   //z = 40  + i;
			   //x = 128 + j;
			   //y = 128 + t;
			   int V1,V2;
			   V1 = (j+t)%40;
			   V2 = (j-t+80000)%40;
			   if((!(V1>20&&V1<24)) &&
			      (!(V2>20&&V2<24)) )
			   //if(!(V>35&&V<40))
			   *get_voxl_color(VolDat1,i,j,t) = 0; 
		      // 	V= (j-t+160)%80;
			  // if(!(V>35&&V<40))
			  // *get_voxl_color(VolDat1,i,j,t) = 0; 
		   }
	} 

	for(j = 20; j<340; j++)
	{
       for( i= 30; i < 330; i++ )
		   for( t= 30 ; t < 330; t++ )
		   {
			   //z = 40  + i;
			   //x = 128 + j;
			   //y = 128 + t;
			   int V1,V2;
			   V1 = (i+t)%40;
			   V2 = (i-t+80000)%40;
			   if((!(V1>20&&V1<24)) &&
			      (!(V2>20&&V2<24)) )
			   //if(!(V>35&&V<40))
			   *get_voxl_color(VolDat1,i,j,t) = 0;  
		      // 	V= (j-t+160)%80;
			  // if(!(V>35&&V<40))
			  // *get_voxl_color(VolDat1,i,j,t) = 0; 
		   }
	} /**/
	
	for(j = 30; j<330; j++)
	{
       for( i= 30; i < 330; i++ )
		   for( t= 20 ; t < 340; t++ )
		   {
			    *get_voxl_color(VolDat1,i,j,t) = 0; 
		   }
	}/**/
	
	for(j = 30; j<330; j++)
	{
       for( i= 30; i < 330; i++ )
		   for( t= 20 ; t < 30; t++ )
		   {
			    if((i<42||i>318)||
		           (j<42||j>318))
				*get_voxl_color(VolDat1,i,j,t) = 8000; 
		   }
	}
	
	CVolumeData VolDat;
	VolDat.FormatF(500,500,360);
	for(i = 0; i<500; i++)
	{
       for( j = 0; j <500; j++ )
		   for( t = 0; t < 360; t++ )
		   {
              *get_voxl_color(VolDat,i,j,t) = 0;
		   }
	}
	
	//x=x'·cos(-θ)+y'·sin(-θ)
    //y=y'·cos(-θ)-x'·sin(-θ)

	 for( t = 0; t < 360; t++ )
	 {
		  float scale = 0.7 + 0.3 *((t-180.0)/180.0)*((t-180.0)/180.0);
		  printf("%f\n",scale);
		  
		 for(i = 0; i<500; i++)
	{
       for( j = 0; j< 500; j++ )
		  
		   {
              float xx,yy,tx,ty,angle;
			  xx = i - 250;
			  yy = j - 250;
			  
			  xx /=scale;
			  yy /=scale;
			  
			  angle  = t;
			  angle *= 3.1415912/90.0 * 2.0/3.0/16.0;
			  //angle = 0;
			  tx = xx * cos(angle) + yy * sin(angle);
			  ty = yy * cos(angle) - xx * sin(angle);
			  
			 
			 // tx *=scale;
			 // ty *=scale;
			  tx += 180.0;
			  ty += 180.0;
			 

			  if(tx<1 || tx > 359 ||ty<1||ty>358)
			  {*get_voxl_color(VolDat,i,j,t) = 0;
			  }
			  {
				  unsigned short* PtPos =  get_voxl_color(VolDat1,int(tx),int(ty),t);
				  float fu = tx - float(int(tx));
				  float fv = ty - float(int(ty));
				  *get_voxl_color(VolDat,i,j,t) =
				  interpolate(PtPos ,int(tx),int(ty),
                   fu,  fv,360,360);
			  }
		   }
	}
	 }
	 /**/
	//VolDat.Save("ten_test");

	//printf("hello 3d world!\n");
	//=============================================================================
 
	CIsoSurface<unsigned short> isurf;
	double nStepSize = 1.0;	// In millimeters.
	int xStepSize = max( 1, int( nStepSize/VolDat.ImgResl[0]+0.5 ) );
	int yStepSize = max( 1, int( nStepSize/VolDat.ImgResl[1]+0.5 ) );
	int zStepSize = max( 1, int( nStepSize/VolDat.ImgResl[2]+0.5 ) );

   isurf.GenerateSurface( VolDat.Buffer, 300, VolDat.ImWidth ,VolDat.ImHeight, VolDat.VolHeight, 
   (float)VolDat.ImgResl[0], (float)VolDat.ImgResl[1], (float)VolDat.ImgResl[2],
	xStepSize, yStepSize, zStepSize );

   int nPoints      = isurf.m_nVertices;
   int nTriangles	= isurf.m_nTriangles;
 
//MeshSmoothing( double *pMeshPoint, char *pPointType, int nMeshPoint, int *pTriangle, 
//				  int nTriangle, int nMaxIter );
//-------------------------------------------------------------------------
string    fn_tbin,fn_pbin,fnameprefix;
fnameprefix ="demoshape";
fn_tbin = fnameprefix+".tbin";
fn_pbin = fnameprefix+".pbin";
FILE* file;
double* VData    = new double[nPoints*3];
double*smooth_buf= new double[nPoints*3];
double*IdxBuffer = new double[nTriangles*3]; 
 
	for (i=0; i<nPoints; i++)
	{
		smooth_buf[3*i+0]	= isurf.m_ppt3dVertices[i][0];
		smooth_buf[3*i+1]	= isurf.m_ppt3dVertices[i][1];
		smooth_buf[3*i+2]	= isurf.m_ppt3dVertices[i][2];
	}

 file= fopen("shape.obj","wt+");
 
 //int cout;

 for (i=0; i<nPoints; i++)
{
	fprintf(file, "v %6.2f  %6.2f  %6.2f\n", isurf.m_ppt3dVertices[i][0], isurf.m_ppt3dVertices[i][1], isurf.m_ppt3dVertices[i][2]);
}

  for (i=0; i<nTriangles; i++)
{
	fprintf(file, "f %i  %i  %i\n", isurf.m_piTriangleIndices[i*3+0]+1, isurf.m_piTriangleIndices[i*3+1]+1, isurf.m_piTriangleIndices[i*3+2]+1);
}

 fclose(file);
	return 1;
}
