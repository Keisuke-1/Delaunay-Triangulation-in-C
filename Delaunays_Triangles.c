#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define max_ele 100
#define max_iter 100
#define PI 3.141592654
#define max_x 0.9
#define max_y 0.9
#define min_x 0.1
#define min_y 0.1
#define tolerance 0.0001
#define MAX_ERROR 1000000



int collinear(float x1, float y1, float x2, float y2, float x3, float y3)
{

    float a = x1 * (y2 - y3) +
            x2 * (y3 - y1) +
            x3 * (y1 - y2);
    

    if (a < tolerance)
    {
        // printf("\n yes colinear \n");
        return 1;
    }
        
    else
    {
        // printf("\n nnot colinear \n");
        return 0;
    }
        
}

int isUniqueElement(int delone_elements[][3],int a,int b,int c,int count_delone_ele)
{
    int flag=1;

    for(int i=0;i<count_delone_ele;i++)
    {
        // printf("\nUnique Func : %d,%d,%d == %d,%d,%d",delone_elements[i][0],delone_elements[i][1],delone_elements[i][2],a,b,c);
        if ((delone_elements[i][0]==a) && (delone_elements[i][1]==b) && (delone_elements[i][2]==c))
        {
            flag=0;
            break;
        }
    }

    return flag;

}

// This function sort 2D array row-wise
int **sortRowWise(int m[][3], int r, int c)
{
    int t = 0;
    
    for (int i = 0; i < r; i++) {
        
        for (int j = 0; j < c; j++) {
            
            for (int k = 0; k < c - j - 1; k++) {
                
                if (m[i][k] > m[i][k + 1]) {
                    
                    t = m[i][k];
                    m[i][k] = m[i][k + 1];
                    m[i][k + 1] = t;
                }
            }
        }
    }

    int **sortedRows;
    sortedRows=malloc(sizeof(int *)* r);

    for(int i=0; i<r; i++)
        sortedRows[i]=malloc(sizeof(int)*c);

    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            sortedRows[i][j]=m[i][j];

    return sortedRows;
}

void findCircle(float x1, float y1, float x2, float y2, float x3, float y3, float *xc, float *yc, float *R) 
{
    float x12 = x1 - x2;
    float x13 = x1 - x3;

    float y12 = y1 - y2;
    float y13 = y1 - y3;

    float y31 = y3 - y1;
    float y21 = y2 - y1;

    float x31 = x3 - x1;
    float x21 = x2 - x1;

    // x1^2 - x3^2
    float sx13 = pow(x1, 2) - pow(x3, 2);

    // y1^2 - y3^2
    float sy13 = pow(y1, 2) - pow(y3, 2);

    float sx21 = pow(x2, 2) - pow(x1, 2);
    float sy21 = pow(y2, 2) - pow(y1, 2);

    float f = (sx13 * x12 + sy13 * x12 + sx21 * x13 + sy21 * x13) / (2 * (y31 * x12 - y21 * x13));
    float g = (sx13 * y12 + sy13 * y12 + sx21 * y13 + sy21 * y13) / (2 * (x31 * y12 - x21 * y13));

    float c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    float h = -g;
    float k = -f;
    float sqr_of_r = h * h + k * k - c;

    // r is the radius
    float r = sqrt(sqr_of_r);

    *xc=h;
    *yc=k;
    *R=r;
}

void isInsideCircle(float xc, float yc, float r,int *result,float x, float y)
{
    float temp=(x-xc)*(x-xc) + (y-yc)*(y-yc) - r*r;

    if(temp>0)
    {
        // printf("\n=======doest not Lies inside circumcircle=======\n");
        *result=0;
    }
    if(temp<=0)
    {
        // printf("\n=======Lies inside circumcircle=======\n");
        *result=1;
    }
}

int anyNodeLiesInside(float nodes[][2],int numNodes,int n1,int n2,int n3)
{

    // printf("\nchecking other nodes inside the triangle and circumcirle\n");

    int count=0;
    for(int i=0;i<numNodes;i++)
    {
        if( (i!=n1) && (i!=n2) && (i!=n3) )
        {
            // printf("\nnode number:[%d]\n",i+1);

            // printf("\nx1=%f, y1=%f, x2=%f, y2=%f, x3=%f, y3=%f, x=%f, y=%f\n",nodes[n1][0],nodes[n1][1], nodes[n2][0],nodes[n2][1], nodes[n3][0],nodes[n3][1], nodes[i][0],nodes[i][1]);
            
            float xc=0,yc=0,r=0;

            findCircle(nodes[n1][0],nodes[n1][1], nodes[n2][0],nodes[n2][1], nodes[n3][0],nodes[n3][1],&xc,&yc,&r);

            // printf("\n circumcircle xc=%f, yc=%f, R=%f\n",xc,yc,r);

            int ans_circle=0;

            isInsideCircle(xc,yc,r,&ans_circle,nodes[i][0],nodes[i][1]);

            if (ans_circle==1)
            {
                count++;
                break;
            }
                
        }
    }

    if(count>0)
        return 1;
    else
        return 0;
}

int **aniticlockwiseNodes(int delone_elements[][3],int count_delone_ele, float nodes[][2])
{
    int reorder[count_delone_ele][3];

    for(int gg=0;gg<count_delone_ele;gg++)
    {
        float xc=0,yc=0,r=0;
        
        int n1=delone_elements[gg][0];
        int n2=delone_elements[gg][1];
        int n3=delone_elements[gg][2];

        findCircle(nodes[n1][0],nodes[n1][1], nodes[n2][0],nodes[n2][1], nodes[n3][0],nodes[n3][1],&xc,&yc,&r);

        // printf("\n circumcircle xc=%f, yc=%f, R=%f\n",xc,yc,r);
        float ang_n1=atan2(nodes[n1][1]-yc,nodes[n1][0]-xc);
        float ang_n2=atan2(nodes[n2][1]-yc,nodes[n2][0]-xc);
        float ang_n3=atan2(nodes[n3][1]-yc,nodes[n3][0]-xc);

        float ele_angles[3][2]={ {n1,ang_n1}, {n2,ang_n2}, {n3,ang_n3} };

        for(int j=0;j<3;j++)
        {
            // printf("\nangle=%f",ele_angles[j][1]);
            if(ele_angles[j][1]<0)
            {
                // printf("\nim here\n");
                ele_angles[j][1]= 2*PI + ele_angles[j][1];
                // printf("\n+angle=%f\n",ele_angles[j][1]);
            }
        }

        // printf("\n===circumcircle ang1=%f, ang2=%f, ang3=%f",ang_n1*180/PI,ang_n2*180/PI,ang_n3*180/PI);
        // for(int j=0;j<3;j++)
        //     printf("\npositive angles %f",ele_angles[j][1]*180/PI);
        // printf("\n");

        float angles[]={ele_angles[0][1],ele_angles[1][1],ele_angles[2][1]};

        // printf("\nele_angles[j][0]=%d,%d,%d\n",ele_angles[0][0],ele_angles[1][0],ele_angles[2][0]);

        int length=3;

        for (int i = 0; i < length; i++) 
        {     
            for (int j = i+1; j < length; j++) 
            {     
                if(angles[i] > angles[j]) 
                {    
                    float temp = angles[i];    
                    angles[i] = angles[j];    
                    angles[j] = temp;    
                }     
            }     
        }    
        
        // printf("\n");    
            
        //Displaying elements of array after sorting    
        // printf("Elements of array sorted in ascending order: \n");    
        // for (int i = 0; i < length; i++) 
        // {     
        //     printf("%f ", angles[i]);    
        // }

        // printf("\n changing [%d][%d][%d] -->",n1+1,n2+1,n3+1);

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                // printf("\n angle(i)=%f,ele_angle(j)=%f\n",angles[i],ele_angles[j][1]);
                if(angles[i]==ele_angles[j][1])
                {
                    // printf("\nim here, ele_angle[%d][0]=%d\n",j,ele_angles[j][0]);
                    reorder[gg][i]=ele_angles[j][0];
                }
            }
        }

        // printf("gg=%d, [%d][%d][%d]\n",gg,reorder[gg][0]+1,reorder[gg][1]+1,reorder[gg][2]+1);

        
    }

    // for(int i=0;i<count_delone_ele;i++)
    //     printf("\nunique delone triangles are [%d] {%d, %d, %d}\n",i+1,reorder[i][0]+1,reorder[i][1]+1,reorder[i][2]+1);


    int **sortedRows;
    sortedRows=malloc(sizeof(int *)* count_delone_ele);

    for(int i=0; i<count_delone_ele; i++)
        sortedRows[i]=malloc(sizeof(int)*3);

    for(int i=0;i<count_delone_ele;i++)
        for(int j=0;j<3;j++)
            sortedRows[i][j]=reorder[i][j];

    return sortedRows;

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int main() 
{
    
    //===================================== INPUT =========================================

    float nodes[][2] = { { 0.0, 0}, { 1, 0 }, { 1, 1 }, { 0,1} ,{.3,.8},{.7,.2}}; // node's coordinate : {x,y}
    
    // ===================================== the Delone loop ===================================================

    int delone_nodes[max_ele][3];
    int delone_count=0;

    int numNodes = sizeof(nodes) / sizeof(nodes[0]);

    printf("\nNumber of nodes: %d\n",numNodes);

    for(int n1=0;n1<numNodes;n1++)
    {
        for(int n2=0;n2<numNodes;n2++)
        {
            for(int n3=0;n3<numNodes;n3++)
            {
                int ans_colinear=collinear(nodes[n1][0],nodes[n1][1],nodes[n2][0],nodes[n2][1],nodes[n3][0],nodes[n3][1]);

                if((n1!=n2) && (n1!=n3) && (n2!=n3) && (ans_colinear!=1))
                {
                    // printf("\n===== current nodes [%d],[%d],[%d] =====\n",n1+1,n2+1,n3+1);

                    int ans=anyNodeLiesInside(nodes,numNodes,n1,n2,n3);

                    if(ans==0)
                    {
                        delone_nodes[delone_count][0]=n1;
                        delone_nodes[delone_count][1]=n2;
                        delone_nodes[delone_count][2]=n3;
                        // printf("\ndelone triangle is {%d, %d, %d}\n",delone_nodes[delone_count][0]+1,delone_nodes[delone_count][1]+1,delone_nodes[delone_count][2]+1);
                        delone_count++;
                        
                    }
                        
                }
            }
        }
    }

    // printf("\nDelone element count is %d\n",delone_count);

    // for(int i=0;i<delone_count;i++)
    // {
    //     printf("\ndelone nodes are [%d]{%d, %d, %d}\n",i+1,delone_nodes[i][0]+1,delone_nodes[i][1]+1,delone_nodes[i][2]+1);
    // }

    int **sortedRows;
    sortedRows=sortRowWise(delone_nodes, delone_count, 3);

    // for(int i=0;i<delone_count;i++)
    //     printf("\nsorted delone nodes are [%d] {%d, %d, %d}\n",i+1,sortedRows[i][0]+1,sortedRows[i][1]+1,sortedRows[i][2]+1);


    // finding unique elements 

    int count_delone_ele=1;

    int delone_elements[delone_count][3];

    delone_elements[0][0]=sortedRows[0][0];
    delone_elements[0][1]=sortedRows[0][1];
    delone_elements[0][2]=sortedRows[0][2];

    for(int i=1;i<delone_count;i++)
    {
        int ans=isUniqueElement(delone_elements,sortedRows[i][0],sortedRows[i][1],sortedRows[i][2],count_delone_ele);
        // printf("\nunique element=%d, unique count=%d\n",ans,count_delone_ele);
        if(ans==1)
        {
            // printf("\n==unique delone nodes is [%d] {%d, %d, %d}\n",i+1,sortedRows[i][0]+1,sortedRows[i][1]+1,sortedRows[i][2]+1);
            delone_elements[count_delone_ele][0]=sortedRows[i][0];
            delone_elements[count_delone_ele][1]=sortedRows[i][1];
            delone_elements[count_delone_ele][2]=sortedRows[i][2];
            count_delone_ele++;
        }
    }

    for(int i=0;i<count_delone_ele;i++)
        printf("\nunique delone triangles are [%d] {%d, %d, %d}\n",i+1,delone_elements[i][0]+1,delone_elements[i][1]+1,delone_elements[i][2]+1);
    
    int **conectivity;

    conectivity=aniticlockwiseNodes(delone_elements,count_delone_ele,nodes);

    for(int i=0;i<count_delone_ele;i++)
        printf("\ndirectionalized delone triangles are [%d] {%d, %d, %d}\n",i+1,conectivity[i][0]+1,conectivity[i][1]+1,conectivity[i][2]+1);

}
