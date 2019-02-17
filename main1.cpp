#include <stdio.h>
#include <stdlib.h>
#include "c24bitmap.h"
#include "c256bitmap.h"
#include "VolRead.h"
#include "cisosurface.h"

int main()
{
	CVolumeData VolDat;
	VolDat.FormatF(360,360,360);

	int i,j,t;
	int x,y,z;

	for(i = 0; i<360; i++)
	{
       for( j = 0; j <360; j++ )
		   for( t = 0; t < 360; t++ )
		   {
              *get_voxl_color(VolDat,i,j,t) = 0;
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
			   *get_voxl_color(VolDat,i,j,t) = 8000; 
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
			   if((!(V1>20&&V1<28)) &&
			      (!(V2>20&&V2<28)) )
			   //if(!(V>35&&V<40))
			   *get_voxl_color(VolDat,i,j,t) = 8000;  
		      // 	V= (j-t+160)%80;
			  // if(!(V>35&&V<40))
			  // *get_voxl_color(VolDat,i,j,t) = 0; 
		   }
	} 

	for(j = 20; j<340; j++)
	{
       for( i= 20; i < 340; i++ )
		   for( t= 20 ; t < 50; t++ )
		   {
			   if(i > 40 && i<300)continue; 
			   if(j > 40 && j<300)continue;
			   *get_voxl_color(VolDat,i,j,t) = 0; 
		   }
	} /**/
	
	for(j = 30; j<330; j++)
       for( i= 30; i < 330; i++ )
		   for( t= 20 ; t < 340; t++ )
		   {
			  *get_voxl_color(VolDat,i,j,t) = 0; 
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
			   if((!(V1>20&&V1<28)) &&
			      (!(V2>20&&V2<28)) )
			   //if(!(V>35&&V<40))
			   *get_voxl_color(VolDat,i,j,t) = 0; 
		      // 	V= (j-t+160)%80;
			  // if(!(V>35&&V<40))
			  // *get_voxl_color(VolDat,i,j,t) = 0; 
		   }
	}
	//VolDat.Save("ten_test");

	//printf("hello 3d world!\n");
	//=============================================================================
 
	CIsoSurface<unsigned short> isurf;
	double nStepSize = 1.0;	// In millimeters.
	int xStepSize = max( 1, int( nStepSize/VolDat.ImgResl[0]+0.5 ) );
	int yStepSize = max( 1, int( nStepSize/VolDat.ImgResl[1]+0.5 ) );
	int zStepSize = max( 1, int( nStepSize/VolDat.ImgResl[2]+0.5 ) );

   isurf.GenerateSurface( VolDat.Buffer, 50, VolDat.ImWidth ,VolDat.ImHeight, VolDat.VolHeight, 
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
