

#include "image_processing.h"
#include <stdio.h>
#include <cstdio>

CImageProcessor::CImageProcessor() {
	for(uint32 i=0; i<3; i++) {
		/* index 0 is 3 channels and indicies 1/2 are 1 channel deep */
		m_proc_image[i] = new cv::Mat();
	}
}

CImageProcessor::~CImageProcessor() {
	for(uint32 i=0; i<3; i++) {
		delete m_proc_image[i];
	}
}

cv::Mat* CImageProcessor::GetProcImage(uint32 i) {
	if(2 < i) {
		i = 2;
	}
	return m_proc_image[i];
}

int CImageProcessor::DoProcess(cv::Mat* image) {
	
	if(!image) return(EINVALID_PARAMETER);	
        
        
        cv::Mat grayImage = image->clone();
        
        
        if(image->channels() > 1){
            cv::cvtColor(*image, grayImage, cv::COLOR_RGB2GRAY);
        }else{
            grayImage = *image;
        }
        
        if (mBkgrImage.empty()){
            mBkgrImage = grayImage.clone();
        }
        
        if (mPrevImage.size() != cv::Size()){
            
            
            //ActImageDx = imfilter(double(Image), DX)
            
            
            
            
            
            cv::Mat diffImage; //create variable difference image
            
            double alpha = 0.9;  //factor for background average
            static int backGrndCntr = 0;
            //if(backGrndCntr < 100){ //estimate the background with the first 100 frames (this way you have to restart the application, if the camera position has changed)
                 cv::addWeighted(mBkgrImage, alpha, grayImage, 1-alpha, 0, mBkgrImage);
                 //backGrndCntr++;
                 //for "official" way, comment out this if statement
            //}
            
            if(backGrndCntr == 100){//let the user know we're done
                printf("\n\n******************************\nBackground analyzed\n******************************\n");
                fflush(stdout);
                backGrndCntr++;
            }
          
            
            cv::absdiff(grayImage, mBkgrImage, diffImage);//calculate the difference image
           
            
            double threshold = 20;//apply a threshold of 20
            cv::Mat binaryImage;
            cv::threshold(diffImage, binaryImage, threshold, 255, CV_THRESH_BINARY);
            
            cv::Mat kernel = cv::Mat::ones(10,10, CV_8UC1); //do a closure on the binary image
            cv::morphologyEx(binaryImage, binaryImage, cv::MORPH_CLOSE,kernel);
            
            //cv::Mat kernel0 = cv::Mat::ones(3,3, CV_8UC1); //apply a linear filter for noise reduction
            //cv::filter2D(binaryImage, binaryImage, -1, kernel0/(float)9 , cv::Point(-1,-1), 0, cv::BORDER_DEFAULT);
            //does not work...
            
            
            
            cv::Mat resultImage = image->clone();
       
           
            std::vector<std::vector<cv::Point> > contours;
            std::vector<cv::Vec4i> hierarchy;
            
            *m_proc_image[1] = binaryImage.clone();
            
            
            
            cv::findContours(binaryImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
            int idx2 = 0;
            for(unsigned int idx=0; idx < contours.size(); idx++){
                //area
                double area = cv::contourArea(contours[idx]);
                //bounding rect
                cv::Rect rect = cv::boundingRect(contours[idx]);
                //center of gravity cm
                cv::Moments moment = cv::moments(contours[idx]);
                double cx = moment.m10 / moment.m00;
                double cy = moment.m01 / moment.m00;
                
                
                if(area > 600){
                    cv::Point2d cent(cx, cy);
                    cv::circle(resultImage, cent, 5, cv::Scalar(128, 0, 0),-1);
                    cv::drawContours(resultImage, contours, idx, cv::Scalar(255), 1, 8);
                    char no[20];
                    sprintf(no, "region = %d", idx2);
                    cv::rectangle(resultImage, rect, cv::Scalar(255,255,0));
                    cv::putText(resultImage, no, cv::Point((int)cx,(int)cy), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,255), 1);
                    printf("region %d: area = %f, cm = (%8.2f, %8.2f)\n", idx2, area, cx, cy);
                    idx2++;
                    fflush(stdout);
                }
            
            
            }
            
            
            *m_proc_image[0] = mBkgrImage.clone();
            
            *m_proc_image[2] = resultImage.clone();
        }
        
        
        mPrevImage = grayImage.clone();
        
       
        


        //cv::subtract(cv::Scalar::all(255), *image,*m_proc_image[0]);
        
      //  cv::imwrite("dx.png", *m_proc_image[0]);
      //  cv::imwrite("dy.png", *m_proc_image[1]);

	return(SUCCESS);
}








