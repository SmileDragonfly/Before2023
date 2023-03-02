#include "Utils.h"
#include "SimpleIni.h"
#include "spdlog/spdlog.h"

bool LoadCamConfiguration(cv::String sFile,
	cv::Mat* extrinMat,
	cv::Mat* intrinMatLeft,
	cv::Mat* intrinMatRight,
	cv::Mat* rotationMatRight2Left,
	cv::Mat* translateMatRight2Left)
{
	CSimpleIniA ini;
	SI_Error err = ini.LoadFile(sFile.c_str());
	if (err < 0)
	{
		spdlog::error("Load ini failed: {}", sFile);
		return false;
	}
	else
	{
		spdlog::info("Load ini success: {}", sFile);
	}

	// Get instrinsic
	const char* fc_left;
	const char* cc_left;
	const char* alpha_c_left;
	const char* kc_left;
	const char* fc_right;
	const char* cc_right;
	const char* alpha_c_right;
	const char* kc_right;
	fc_left = ini.GetValue("CAM_INSTRINSIC", "fc_left", "");
	cc_left = ini.GetValue("CAM_INSTRINSIC", "cc_left", "");
	alpha_c_left = ini.GetValue("CAM_INSTRINSIC", "alpha_c_left", "");
	kc_left = ini.GetValue("CAM_INSTRINSIC", "kc_left", "");
	fc_right = ini.GetValue("CAM_INSTRINSIC", "fc_right", "");
	cc_right = ini.GetValue("CAM_INSTRINSIC", "cc_right", "");
	alpha_c_right = ini.GetValue("CAM_INSTRINSIC", "alpha_c_right", "");
	kc_right = ini.GetValue("CAM_INSTRINSIC", "kc_right", "");
	// Get extrinsic
	const char* om;
	const char* T;
	om = ini.GetValue("CAM_EXSTRINSIC", "om", "");
	T = ini.GetValue("CAM_EXSTRINSIC", "T", "");

	// Push all value to vector
	std::vector<float> vfc_left;
	std::vector<float> vcc_left;
	std::vector<float> valpha_c_left;
	std::vector<float> vkc_left;
	std::vector<float> vfc_right;
	std::vector<float> vcc_right;
	std::vector<float> valpha_c_right;
	std::vector<float> vkc_right;
	std::vector<float> vom;
	std::vector<float> vT;
	GetValues(fc_left, vfc_left);
	GetValues(cc_left, vcc_left);
	GetValues(alpha_c_left, valpha_c_left);
	GetValues(kc_left, vkc_left);
	GetValues(fc_right, vfc_right);
	GetValues(cc_right, vcc_right);
	GetValues(alpha_c_right, valpha_c_right);
	GetValues(kc_right, vkc_right);
	GetValues(om, vom);
	GetValues(T, vT);

	// Push values to exstrinsic matrix 4*4
	extrinMat->create(4, 4, CV_32FC1);
	extrinMat->at<float>(0, 0) = vom.at(0);
	extrinMat->at<float>(0, 1) = vom.at(1);
	extrinMat->at<float>(0, 2) = vom.at(2);
	extrinMat->at<float>(1, 0) = vom.at(3);
	extrinMat->at<float>(1, 1) = vom.at(4);
	extrinMat->at<float>(1, 2) = vom.at(5);
	extrinMat->at<float>(2, 0) = vom.at(6);
	extrinMat->at<float>(2, 1) = vom.at(7);
	extrinMat->at<float>(2, 2) = vom.at(8);

	extrinMat->at<float>(0, 3) = vT.at(0);
	extrinMat->at<float>(1, 3) = vT.at(1);
	extrinMat->at<float>(2, 3) = vT.at(2);

	extrinMat->at<float>(3, 0) = 0;
	extrinMat->at<float>(3, 1) = 0;
	extrinMat->at<float>(3, 2) = 0;
	extrinMat->at<float>(3, 3) = 1;
	std::cout << "extrinMat= " << std::endl << " " << *extrinMat << std::endl << std::endl;

	// Push values to instrinsic matrixs 3*3
	intrinMatLeft->create(3, 3, CV_32FC1);
	intrinMatLeft->setTo(0);
	intrinMatLeft->at<float>(0, 0) = vfc_left.at(0);
	intrinMatLeft->at<float>(1, 1) = vfc_left.at(1);
	intrinMatLeft->at<float>(0, 2) = vcc_left.at(0);
	intrinMatLeft->at<float>(1, 2) = vcc_left.at(1);
	intrinMatLeft->at<float>(2, 2) = 1;
	std::cout << "intrinMatLeft= " << std::endl << " " << *intrinMatLeft << std::endl << std::endl;

	intrinMatRight->create(3, 3, CV_32FC1);
	intrinMatRight->setTo(0);
	intrinMatRight->at<float>(0, 0) = vfc_right.at(0);
	intrinMatRight->at<float>(1, 1) = vfc_right.at(1);
	intrinMatRight->at<float>(0, 2) = vcc_right.at(0);
	intrinMatRight->at<float>(1, 2) = vcc_right.at(1);
	intrinMatRight->at<float>(2, 2) = 1;
	std::cout << "intrinMatRight= " << std::endl << " " << *intrinMatRight << std::endl << std::endl;

	// Push values to rotation matrix 3*3
	rotationMatRight2Left->create(3, 3, CV_32FC1);
	rotationMatRight2Left->at<float>(0, 0) = vom.at(0);
	rotationMatRight2Left->at<float>(0, 1) = vom.at(1);
	rotationMatRight2Left->at<float>(0, 2) = vom.at(2);
	rotationMatRight2Left->at<float>(1, 0) = vom.at(3);
	rotationMatRight2Left->at<float>(1, 1) = vom.at(4);
	rotationMatRight2Left->at<float>(1, 2) = vom.at(5);
	rotationMatRight2Left->at<float>(2, 0) = vom.at(6);
	rotationMatRight2Left->at<float>(2, 1) = vom.at(7);
	rotationMatRight2Left->at<float>(2, 2) = vom.at(8);
	std::cout << "rotationMatRight2Left= " << std::endl << " " << *rotationMatRight2Left << std::endl << std::endl;

	// Push values to translation matrix 3*1
	translateMatRight2Left->create(3, 1, CV_32FC1);
	translateMatRight2Left->at<float>(0, 0) = vT.at(0);
	translateMatRight2Left->at<float>(1, 0) = vT.at(1);
	translateMatRight2Left->at<float>(2, 0) = vT.at(2);
	std::cout << "translateMatRight2Left= " << std::endl << " " << *translateMatRight2Left << std::endl << std::endl;

	return true;
}

bool GetValues(std::string sIn, std::vector<float>& vOut)
{
	// Find 1st character ] in string
	size_t pos = sIn.find("]");
	std::string sSub = sIn.substr(0, pos + 1);
	sSub.erase(0, sSub.find_first_not_of("[] ")); // left trim
	sSub.erase(sSub.find_last_not_of("[] ") + 1); // right trim
	std::stringstream ss(sSub);
	while(!ss.eof())
	{
		float tempVal = 0;
		ss >> tempVal;
		vOut.push_back(tempVal);
	}
	if (vOut.size() == 0)
	{
		return false;
	}
	return true;
}

cv::Point GetMappingPoint(cv::Point point, cv::Mat instrinMat, cv::Mat instrinMatMapping, cv::Mat rotationMat, cv::Mat translateMat)
{
	cv::Point retPoint;
	cv::Mat pixelMat(3, 1, CV_32FC1);
	pixelMat.at<float>(0, 0) = point.x;
	pixelMat.at<float>(1, 0) = point.y;
	pixelMat.at<float>(2, 0) = 1;
	cv::Mat invIntrinMat = instrinMat.inv();
	std::cout << "pixelMat= " << std::endl << " " << pixelMat << std::endl << std::endl;
	std::cout << "invIntrinMat= " << std::endl << " " << invIntrinMat << std::endl << std::endl;
	cv::Mat camMat = invIntrinMat * pixelMat;
	cv::Mat mappingCamMat = rotationMat.inv() * (camMat - translateMat);
	cv::Mat mappingPixelMat = instrinMatMapping * mappingCamMat;
	std::cout << "camMat= " << std::endl << " " << camMat << std::endl << std::endl;
	std::cout << "mappingCamMat= " << std::endl << " " << mappingCamMat << std::endl << std::endl;
	std::cout << "mappingPixelMat= " << std::endl << " " << mappingPixelMat << std::endl << std::endl;
	retPoint.x = mappingPixelMat.at<float>(0, 0) / mappingPixelMat.at<float>(2, 0);
	retPoint.y = mappingPixelMat.at<float>(1, 0) / mappingPixelMat.at<float>(2, 0);
	return retPoint;
}
