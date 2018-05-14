#include "COLLADA2GLTFOptions.h"


using namespace COLLADA2GLTF;

/**Создать экземпляр опций GLTF */
std::shared_ptr<dgn::gltf::GltfOptions> dgn::gltf::GltfOptions::create(){
	return std::shared_ptr<dgn::gltf::GltfOptions>(new COLLADA2GLTF::Options());
}

std::string Options::getInputPath(){
	return this->inputPath;
}

void Options::setInputPath(const std::string & value){
	this->inputPath = value;
}

/**Папка относительно которой задаются пути */
std::string Options::getBasePath(){
	return this->basePath;
}

void Options::setBasePath(const std::string & value){
	this->basePath = value;
}

/**Путь для вывода glTF файла */
std::string Options::getOutputPath(){
	return this->outputPath;
}

void Options::setOutputPath(const std::string & value){
	this->outputPath = value;
}

bool Options::getEmbeddedBuffers(){
	return this->embeddedBuffers;
}

void Options::setEmbeddedBuffers(bool value){
	this->embeddedBuffers = value;
}

bool Options::getEmbeddedTexture(){
	return this->embeddedTextures;
}

void Options::setEmbeddedTexture(bool value){
	this->embeddedTextures = value;
}

bool Options::getEmbeddedShaders(){
	return this->embeddedShaders;
}

void Options::setEmbeddedShaders(bool value){
	this->embeddedShaders = value;
}

bool Options::getBinary(){
	return this->binary;
}

void Options::setBinary(bool value){
	this->binary = value;
}

bool Options::getMaterialsCommon(){
	return this->materialsCommon;
}

void Options::setMaterialsCommon(bool value){
	this->materialsCommon = value;
}

bool Options::getGlSl(){
	return this->glsl;
}

void Options::setGlSl(bool value){
	this->glsl = value;
}

bool Options::getDoubleSided(){
	return this->doubleSided;
}

void Options::setDoubleSided(bool value){
	this->doubleSided = value;
}

bool Options::getSpecularGlossiness(){
	return this->specularGlossiness;
}

void Options::setSpecularGlossiness(bool value){
	this->specularGlossiness = value;
}

std::string Options::getVersion(){
	return this->version;
}

void Options::setVersion(const std::string & value){
	this->version = value;
}

std::vector<std::string> Options::getMetallicRoughnessTexturePaths(){
	return this->metallicRoughnessTexturePaths;
}

void Options::setMetallicRoughnessTexturePaths(const std::vector<std::string> & value){
	this->metallicRoughnessTexturePaths = value;
}

bool Options::getDracoCompression(){
	return this->dracoCompression;
}

void Options::setDracoCompression(bool value){
	this->dracoCompression = value;
}


int32_t Options::getPositionQuantizationBits(){
	return this->positionQuantizationBits;
}

void Options::setPositionQuantizationBits(int32_t value){
	this->positionQuantizationBits = value;
}

int32_t Options::getNormalQuantizationBits(){
	return this->normalQuantizationBits;
}

void Options::setNormalQuantizationBits(int32_t value){
	this->normalQuantizationBits = value;
}

int32_t Options::getTexcoordQuantizationBits(){
	return this->texcoordQuantizationBits;
}

void Options::setTexcoordQuantizationBits(int32_t value){
	this->texcoordQuantizationBits = value;
}

int32_t Options::getColorQuantizationBits(){
	return this->colorQuantizationBits;
}

void Options::setColorQuantizationBits(int32_t value){
	this->colorQuantizationBits = value;
}

int32_t Options::getJointQuantizationBits(){
	return this->jointQuantizationBits;
}

void Options::setJointQuantizationBits(int32_t value){
	this->jointQuantizationBits = value;
}