/*
 * Vision_Control.h
 *
 *  Created on: 10 Apr 2013
 *      Author: Eugene Gilmore
 */

#ifndef VisionControlStatus_DEFINED
#define VisionControlStatus_DEFINED

#include <gu_util.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <stdint.h>
#include <bitset>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"

enum Resolutions {
	QQVGA,  // 160 x 120
	QVGA,   // 320 x 240
	VGA,    // 640 x 480
	HD_4VGA    // 1280x960
};

class ResolutionType {
private:
        PROPERTY(Resolutions, resolution)
public:
        ResolutionType(Resolutions res = VGA): _resolution(res) {}

        /** Deprecated: use standard getter resolution() instead! */
        int getResolution() const { return _resolution; } // XXX: deprecated, will be phased out!!!

        /** get the width of the current resolution */
        int width() const
        {
                static const int Widths[] = {160, 320, 640, 1280};
                return Widths[_resolution];
        }
    
        /** get the height of the current resolution */
        int height() const
        {
                static const int Heights[] = {120, 240, 480, 960};
                return Heights[_resolution];
        }

        int Width() const { return width(); }           // XXX: deprecated, use width() instead!
        int Height() const { return height(); }         // XXX: deprecated, use height() instead!
};

#pragma clang diagnostic pop

enum VisionCamera {
	Top,
	Bottom
};

enum NamedPipeline {
	Soccer,
        OpenChallenge,
		Streaming
};

enum StreamingType {
	Normal,
	Classified,
	Recognized //NYI
};

enum CalibrationFile {
	Calibration0,
	Calibration1,
	Calibration2,
	Calibration3
};

enum SaveFileType {
    AI2,
    JPG
};

enum VisionMessages {
		Resolution,
		PipelineRunning,
		SelectedCamera,
		SaveImage,
		SaveClassifiedImage,
		ActivatePipeline,
		StreamingSource,
		ConservativeMode,
		ImageInput,
		LoadCalibration,
		PipelineRunOnce,
		JPEGStreamQuality,
		JPEGStreamStride,
		HorizionFactor,
		NUMBER_VISION_MESSAGES
};

static const char* Commands[] = {"RESOLUTION", "RUNPIPELINE", "SELECTCAMERA", "SAVEIMAGE",
		"SAVECLASSIFIEDIMAGE", "ACTIVATEPIPELINE", "STREAMINGSOURCE", "CONSERVATIVEMODE",
		"IMAGEINPUT", "LOADCALIBRATION", "RUNPIPELINEONCE", "JPEGSTREAMQUALITY", "JPEGSTREAMSTRIDE", "HORIZIONFACTOR", "Undefined"};
static const char* Statuses[] = {"Resolution", "PipelineRunning", "SelectedCamera", "SaveImage",
		"SaveClassifiedImage", "ActivePipeline", "StreamingSource", "ConservativeMode",
		"ImageInput", "CalibrationLoaded", "PipelineRunningOnce", "JPEGSTREAMQUALITY", "JPEGSTREAMSTRIDE", "HorizionFactor", "FrameRate"};

static const char* ResolutionStrings[] = {"QQVGA", "QVGA", "VGA", "HD"};
static const Resolutions ResolutionValues[] = {QQVGA, QVGA, VGA, HD_4VGA};
static const char* CameraStrings[] = {"TOP", "BOTTOM"};
static const VisionCamera CameraValues[] = {Top, Bottom};
static const char* PipelineStrings[] = {"SOCCER", "OPENCHALLENGE", "STREAMING"};
static const NamedPipeline PipelineValues[] = {Soccer, OpenChallenge, Streaming};
static const char* StreamingSourceStrings[] = {"NORMAL", "CLASSIFIED", "RECOGNIZED"};
static const StreamingType StreamingSourceValues[] = {Normal, Classified, Recognized};
static const char* CalibrationStrings[] = {"CALIBRATION0", "CALIBRATION1", "CALIBRATION2", "CALIBRATION3"};
static const CalibrationFile CalibrationValues[] = {Calibration0, Calibration1, Calibration2, Calibration3};
static const char* BoolStrings[] = {"YES", "NO"};
static const bool BoolValues[] = {true, false};
static const char* SaveImageStrings[] = {"AI2", "JPG"};
static const SaveFileType SaveImageValues[] = {AI2, JPG};

static const char** MessageParamaters[] = {ResolutionStrings, BoolStrings, CameraStrings, SaveImageStrings,
		BoolStrings, PipelineStrings, StreamingSourceStrings, BoolStrings, BoolStrings, CalibrationStrings, BoolStrings};
static const uint MessageParamaterSizes[] = {sizeof(ResolutionStrings)/sizeof(char*), sizeof(BoolStrings)/sizeof(char*),
                                                sizeof(CameraStrings)/sizeof(char*), sizeof(SaveImageStrings)/sizeof(char*),
                                                sizeof(BoolStrings)/sizeof(char*), sizeof(PipelineStrings)/sizeof(char*),
                                                sizeof(StreamingSourceStrings)/sizeof(char*), sizeof(BoolStrings)/sizeof(char*),
                                                sizeof(BoolStrings)/sizeof(char*), sizeof(CalibrationStrings)/sizeof(char*),
						sizeof(BoolStrings)/sizeof(char*)};

namespace guWhiteboard
{

class VisionControlStatus {
public:
	VisionControlStatus() {
			memset(this, 0, sizeof(*this));
		}

	VisionControlStatus(std::string s) {
		memset(this, 0, sizeof(*this));
		size_t n;
		std::string command;
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		for (unsigned i = 0; i<NUMBER_VISION_MESSAGES; ++i) {
			command = std::string(Commands[i]);
			n = s.find(command);
			if (n!=std::string::npos) {
                                std::string t = s.substr(n+command.length()+1);
                                if(i == HorizionFactor) {
                                    //get value
                                    set_horizionValue(static_cast<float>(atof(t.substr(0, t.find_first_not_of("-0123456789.")).c_str())));
                                    continue;
                                }
								if(i == JPEGStreamQuality) {
                                    //get value
                                    set_jpegStreamQuality(atoi(t.substr(0, t.find_first_not_of("-0123456789.")).c_str()));
                                    continue;
                                }
								if(i == JPEGStreamStride) {
                                    //get value
                                    set_jpegStreamStride(atoi(t.substr(0, t.find_first_not_of("-0123456789.")).c_str()));
                                    continue;
                                }
				for(uint j = 0; j<MessageParamaterSizes[i]; ++j) {
					if(t.compare(0, strlen(MessageParamaters[i][j]), MessageParamaters[i][j]) == 0) {
						switch(i) {
						case Resolution:
							set_resolution(ResolutionValues[j]);
							break;
						case PipelineRunning:
							set_pipelineRunning(BoolValues[j]);
							break;
						case SelectedCamera:
							set_selectedCamera(CameraValues[j]);
							break;
						case SaveImage:
							set_saveImage(SaveImageValues[j]);
							break;
						case SaveClassifiedImage:
							set_saveClassifiedImage(BoolValues[j]);
							break;
						case ActivatePipeline:
							set_activatePipeline(PipelineValues[j]);
							break;
						case StreamingSource:
							set_streamingSource(StreamingSourceValues[j]);
							break;
						case ConservativeMode:
							set_conservativeMode(BoolValues[j]);
							break;
						case ImageInput:
							set_imageInput(BoolValues[j]);
							break;
						case LoadCalibration:
							set_loadCalibration(CalibrationValues[j]);
							break;
						case PipelineRunOnce:
							set_pipelineRunOnce(BoolValues[j]);
						}
					}
				}
			}
		}
	}

	std::string description() const
        {
		std::stringstream result;
                
		if(resolution_mask())
			result << Statuses[0] << "=" << ResolutionStrings[resolution().getResolution()] << " ";//fixme
		if(pipelineRunning_mask())
			result << Statuses[1] << "=" << BoolStrings[pipelineRunning()?0:1] << " ";
		if(selectedCamera_mask())
			result << Statuses[2] << "=" << CameraStrings[selectedCamera()] << " ";
		if(saveImage_mask())
			result << Statuses[3] << "=" << BoolStrings[saveImage()?0:1] << " ";
		if(saveClassifiedImage_mask())
			result << Statuses[4] << "=" << BoolStrings[saveClassifiedImage()?0:1] << " ";
		if(activatePipeline_mask())
			result << Statuses[5] << "=" << PipelineStrings[activatePipeline()] << " ";
		if(streamingSource_mask())
			result << Statuses[6] << "=" << StreamingSourceStrings[streamingSource()] << " ";
		if(conservativeMode_mask())
			result << Statuses[7] << "=" << BoolStrings[conservativeMode()?0:1] << " ";
		if(imageInput_mask())
			result << Statuses[8] << "=" << BoolStrings[imageInput()?0:1] << " ";
		if(jpegStreamQuality_mask())
			result << Statuses[9] << "=" << jpegStreamQuality();
		if(jpegStreamStride_mask())
			result << Statuses[10] << "=" << jpegStreamStride();
		if(loadCalibration_mask())
			result << Statuses[11] << "=" << CalibrationStrings[loadCalibration()] << " ";
		if(pipelineRunOnce_mask())
			result << Statuses[12] << "=" << BoolStrings[pipelineRunOnce()?0:1] << " ";
                if(frameRate_mask())
                        result << Statuses[14] << "=" << frameRate() << " ";
                if(horizionValue_mask())
                    result << Statuses[13] << "=" << horizionValue();
		return result.str();
	}

	VisionControlStatus operator+=(VisionControlStatus a) {
		if(a.resolution_mask())
			this->set_resolution(a.resolution());
		if(a.pipelineRunning_mask())
			this->set_pipelineRunning(a.pipelineRunning());
		if(a.selectedCamera_mask())
			this->set_selectedCamera(a.selectedCamera());
		if(a.saveImage_mask())
			this->set_saveImage(a.saveImage());
		if(a.saveClassifiedImage_mask())
			this->set_saveClassifiedImage(a.saveClassifiedImage());
		if(a.activatePipeline_mask())
			this->set_activatePipeline(a.activatePipeline());
		if(a.streamingSource_mask())
			this->set_streamingSource(a.streamingSource());
		if(a.conservativeMode_mask())
			this->set_conservativeMode(a.conservativeMode());
		if(a.imageInput_mask())
			this->set_imageInput(a.imageInput());
		if(a.loadCalibration_mask())
			this->set_loadCalibration(a.loadCalibration());
		if(a.pipelineRunOnce_mask())
			this->set_pipelineRunOnce(a.pipelineRunOnce());
		if(a.jpegStreamQuality_mask())
			this->set_jpegStreamQuality(a.jpegStreamQuality());
		if(a.jpegStreamStride_mask())
			this->set_jpegStreamStride(a.jpegStreamStride());
        set_openChallengeStep(a.openChallengeStep());
		return *this;
	}

	CONTROLLED_PROPERTY(ResolutionType, resolution)
	CONTROLLED_PROPERTY(bool, pipelineRunning)
	CONTROLLED_PROPERTY(VisionCamera, selectedCamera)
	CONTROLLED_PROPERTY(SaveFileType, saveImage)
	CONTROLLED_PROPERTY(bool, saveClassifiedImage)
	CONTROLLED_PROPERTY(NamedPipeline, activatePipeline)
	CONTROLLED_PROPERTY(StreamingType, streamingSource)
	CONTROLLED_PROPERTY(bool, conservativeMode)
	CONTROLLED_PROPERTY(bool, imageInput)
	CONTROLLED_PROPERTY(int, jpegStreamQuality)
	CONTROLLED_PROPERTY(int, jpegStreamStride)
	CONTROLLED_PROPERTY(CalibrationFile, loadCalibration)
        CONTROLLED_PROPERTY(float, horizionValue)
        CONTROLLED_PROPERTY(int, frameRate)
	CONTROLLED_PROPERTY(bool, pipelineRunOnce)

	CONTROL_BIT(resolution)
	CONTROL_BIT(pipelineRunning)
	CONTROL_BIT(selectedCamera)
	CONTROL_BIT(saveImage)
	CONTROL_BIT(saveClassifiedImage)
	CONTROL_BIT(activatePipeline)
	CONTROL_BIT(streamingSource)
	CONTROL_BIT(conservativeMode)
	CONTROL_BIT(imageInput)
	CONTROL_BIT(jpegStreamQuality)
	CONTROL_BIT(jpegStreamStride)
	CONTROL_BIT(loadCalibration)
        CONTROL_BIT(horizionValue)
        CONTROL_BIT(frameRate)
	CONTROL_BIT(pipelineRunOnce)
		
	PROPERTY(uint8_t, openChallengeStep)

};
}

#pragma clang diagnostic pop

#endif /* VisionController_DEFINED */
