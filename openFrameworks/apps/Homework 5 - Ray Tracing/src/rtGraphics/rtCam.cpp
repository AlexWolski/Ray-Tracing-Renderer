#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	rtCam::rtCam(ofNode transform) : transform(transform)
	{
	}

	rtCam::rtCam(ofVec3f position, ofVec3f rotation)
	{
		transform.setPosition(position);
		transform.setOrientation(ofQuaternion(rotation));
	}

	///Destructor
	rtCam::~rtCam()
	{
		deleteFrameBuffer();
	}

	///Buffer Methods
	//Creates a 3D image buffer array based on the window size
	void rtCam::createFrameBuffer()
	{
		createFrameBuffer(ofGetWindowWidth(), ofGetWindowHeight());
	}

	//Creates a 3D image buffer array
	void rtCam::createFrameBuffer(int width, int height)
	{
		//If a frame buffer already exists, delete it
		if (frameBuffer != NULL)
			deleteFrameBuffer();

		//Save the dimensions of the buffer
		bufferWidth = width;
		bufferHeight = height;

		//Create a triple pointer and allocate an array for the columns of the image
		char*** array3D = new char**[width];

		//Loop through the columns of the image
		for (int col = 0; col < width; col++)
		{
			//Allocate arrays for each row of the image
			array3D[col] = new char*[height];

			//Allocate an array for the RGB values. The default color is black
			for (int row = 0; row < height; row++)
				array3D[col][row] = new char[3] {0, 0, 0};
		}

		//Create a shared pointer to the 3D array and save it
		frameBuffer = make_shared<char***>(array3D);
	}

	//Deallocate the memory used for the frame buffer
	void rtCam::deleteFrameBuffer()
	{
		for (int col = 0; col < bufferWidth; col++)
		{
			//Delete the row arrays
			for (int row = 0; row < bufferHeight; row++)
				delete[] frameBuffer.get()[col][row];

			//Delete the column arrays
			delete[] frameBuffer.get()[col];
		}
	}

	///Scene Methods
	shared_ptr<rtScene> rtCam::getScene()
	{
		return scene;
	}

	void rtCam::setScene(shared_ptr<rtScene> scene)
	{
		this->scene = scene;
	}

	///Camera Methods
	void rtCam::setFov(float fov)
	{
		this->fov = fov;
	}

	float rtCam::getFov()
	{
		return fov;
	}

	void rtCam::render()
	{

	}

}