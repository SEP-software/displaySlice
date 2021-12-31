#include "byte_buffer.h"
#include<cassert>
using namespace SEP;
//Initialize and read in buffer
byte_buffer::byte_buffer(std::shared_ptr<paramObj>p, std::shared_ptr<hypercube>h,std::shared_ptr<io_func>i,int in,std::vector<int>&nwbuf, std::vector<int>&fwbuf){
 
    set_basics(p,h,i,in);


     window_to_local(nwbuf,fwbuf);
    cbuf=new unsigned char [n123_buf];
    std::vector<int> nwio(8,1),fwio(8,1),nloop(8,1);
    int ndim;
    calc_read_loop(nwbuf,fwbuf,nwio,fwio,nloop,ndim);
  read_buffer(nwbuf,fwbuf,nwio,fwio,ndim,nloop);
 }
void byte_buffer::read_buffer(std::vector<int> &nwbuf,std::vector<int> &fwbuf,
std::vector<int> &nwio,std::vector<int> &fwio, int ndim ,std::vector<int> &nloop){
 

   long long nblock=1;
   long long block=1;
   for(int i=0; i< ndim; i++) block=block*(long long) nwbuf[i];
   for(int i=0; i< ndim; i++){
     nblock=nblock*(long long)nwio[i];
   }

   

   long long min_read=500*1000*1000;

   long long total=1;
   block=1;
   std::vector<axis> axes=hyper_io->getAxes();
   for(int i=axes.size();i < 8; i++) axes.push_back(axis(1));
   for(int i=0; i< 8; i++) {
     block=block*(long long)nwio[i];
     total=total*axes[i].n;
   }
   int nread=(min_read/block)+1;
   if(nread*block > total) nread=total/block;
   float *tflt=new float[nread*block];
  // unsigned char *tbuf=new unsigned char [nblock];
   int iread=0;
   std::vector<int> fsend(8,0); ;fsend[0]=fwbuf[0];        fsend[1]=fwio[1];
   for(int i7=0; i7 < nloop[7];i7++){     fsend[7]=fwio[7]+i7;
    for(int i6=0; i6 < nloop[6];i6++){   fsend[6]=fwio[6]+i6;
     for(int i5=0; i5 < nloop[5];i5++){   fsend[5]=fwio[5]+i5;
      for(int i4=0; i4 < nloop[4];i4++){  fsend[4]=fwio[4]+i4;
       for(int i3=0; i3 < nloop[3];i3++){ fsend[3]=fwio[3]+i3;
        for(int i2=0; i2 < nloop[2];i2++){fsend[2]=fwio[2]+i2;
         for(int i1=0; i1 < nloop[1];i1++){fsend[1]=fwio[1]+i1;
          if(iread<nread-1 && io->not_byte()){
             io->read_block_float(nwio,fsend,&tflt[iread*block]);

          }
          else if(iread==nread-1&& io->not_byte()){
            io->read_block_float(nwio,fsend,&tflt[iread*block]);
            io->set_clip(tflt,inum,nread*block);
             io->return_clips(&bclip,&eclip);

  
//            for(int i=0; i < nread; i++){
              io->convert_to_byte(tflt,0,cbuf,0,nread*block);
       //       for(int ii=0;ii<8;ii++) floop[ii]=fsave[i].fs[ii];
        //      off+=resize_buffer(nwbuf,fwbuf,nwio,floop,ndim,
          //     off,(unsigned char*)tbuf,(unsigned char*)cbuf,(int)sizeof(unsigned char));
//               off+=block;
 //           }           
         //   delete [] tflt;
       //     tflt=0;
          }
          else{
            io->read_block_byte(nwio,fsend,(cbuf+(long long)iread*(long long)block));
          //  off+=resize_buffer(nwbuf,fwbuf,nwio,fsend,ndim,
        //     off,(unsigned char*)tbuf,(unsigned char*)cbuf,(int)sizeof(unsigned char));
  
//            off+=block;
         }
         iread+=1;
        }
       }
      }
     }
    }
   }      
 }
if(tflt!=0) delete [] tflt;
 calc_histo();

 io->return_clips(&bclip,&eclip);
}
unsigned char *byte_buffer::get_char_data(std::shared_ptr<orient_cube>pos, int iax1, int f1,
int e1,
  int iax2, int f2, int e2){

   if(!hold[iax1] || !hold[iax2]) {
         fprintf(stderr,"requested axis=%d and %d \n",iax1,iax2);
         _par->error("Internal error don't hold axes requested");
   }
   if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }
   int n1=abs(e1-f1), n2=abs(e2-f2);
   std::shared_ptr<longTensor2D>index=pos->getIndexMapPtr(iax1,iax2,f1,e1,f2,e2,0);
 
     
   unsigned char *out=get_char_data(pos,n1*n2,index);
   
   return out;
}


unsigned char *byte_buffer::get_char_data(std::shared_ptr<orient_cube>pos,int n, long long *mp){
   unsigned char *out=new unsigned char[n];
  if(!pos){;}
   long long *index=grid_to_index(n,mp);
   for(int i=0 ; i < n; i++){
     //fprintf(stderr,"LOOK WHAT %d %lld \n",i,index[i]);
      if(index[i] <0) {
         out[i]=0;
      }
      else{
      
        out[i]=cbuf[index[i]];
      }
   }
    delete [] index;
    return out;
  }   
float *byte_buffer::get_float_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,int f2, int e2){
    
    
    if(!hold[iax1] || !hold[iax2]) _par->error("Internal error don't hold axes requested");
       if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }
   
    if(pos->get_rotate() && (!hold[pos->rot_ax[0]] || !hold[pos->rot_ax[1]])){
      fprintf(stderr,"Must hold rotated axes. Defaulting to no rotation.\n");
      pos->set_no_rotate();
   }
   int n1=abs(e1-f1), n2=abs(e2-f2);
   std::shared_ptr<longTensor2D>index=pos->getIndexMapPtr(iax1,iax2,f1,e1,f2,e2,0);
   float *out=get_float_data(n1*n2,index);
   return out;
   
}

float *byte_buffer::get_float_data(int n, long long *mp){
     float *out=new float [n];
      long long *index=grid_to_index(n,mp);

    float bclip,eclip;
    io->return_clips(&bclip,&eclip);
      float d=(eclip-bclip)/256;
      for(int i=0 ; i < n; i++){
      if(index[i] <0) {
         out[i]=bclip;
      }
      else{
        out[i]=bclip+d*(float)cbuf[index[i]];
      }
   }

    delete [] index;
    return out;
  }
void byte_buffer::calc_histo(){
   
   long long count[256];
   for(int i=0; i < 256; i++) count[i]=0;
   
   for(long long i=0; i < n123_buf; i++) count[cbuf[i]]++;
   long long mym=0;
   for(int i=0; i < 256; i++){
     if(count[i] > mym) mym=count[i];
   }
   for(int i=0; i < 256; i++){
     histo[i]=(float)count[i]/(float)mym;
   }
 }
float byte_buffer::get_value(std::shared_ptr<orient_cube>pos){
   return bclip+((float)(eclip-bclip)/256.0*cbuf[point_to_local(pos)]);
 
 }
