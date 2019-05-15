/*! @file image_processing.h
 * @brief Image Manipulation class
 */

#ifndef IMAGE_PROCESSING_H_
#define IMAGE_PROCESSING_H_

#include "opencv.hpp"//was:"/OpenCV3.0.0/arm-obj_rel/include/opencv2/opencv.hpp"

#include "includes.h"
#include "camera.h"



class CImageProcessor {
public:
	CImageProcessor();
	~CImageProcessor();
	
	int DoProcess(cv::Mat* image);

	cv::Mat* GetProcImage(uint32 i);

private:
	cv::Mat* m_proc_image[3];/* we have three processing images for visualization available */
        cv::Mat mPrevImage;
        cv::Mat mBkgrImage;
};


#endif /* IMAGE_PROCESSING_H_ */
