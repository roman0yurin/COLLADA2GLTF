#include "COLLADA2GLTFWriter.h"

#include "rapidjson/document.h"

#include "ahoy/ahoy.h"
#include "Utils.h"

using namespace ahoy;
using namespace std::experimental::filesystem;



int main(int argc, const char **argv) {
	GLTF::Asset* asset = new GLTF::Asset();
	COLLADA2GLTF::Options* options = new COLLADA2GLTF::Options();

	bool separate;
	bool separateTextures;

	Parser* parser = new Parser();
	parser->name("COLLADA2GLTF")->usage("./COLLADA2GLTF input.dae output.gltf [options]");

	parser->define("i", &options->inputPath)
		->alias("input")
		->description("path of the input COLLADA file")
		->index(0)
		->require();

	parser->define("o", &options->outputPath)
		->alias("output")
		->description("path of the output glTF file")
		->index(1);

	parser->define("basePath", &options->basePath)
		->description("resolve external uris using this as the reference path");

	parser->define("s", &separate)
		->alias("separate")
		->defaults(false)
		->description("output separate binary buffer, shaders, and textures");

	parser->define("t", &separateTextures)
		->alias("separateTextures")
		->defaults(false)
		->description("output images separately, but embed buffers and shaders");

	parser->define("b", &options->binary)
		->alias("binary")
		->defaults(false)
		->description("output binary glTF");

	parser->define("g", &options->glsl)
		->alias("glsl")
		->defaults(false)
		->description("output materials with glsl shaders using the KHR_technique_webgl extension");

	parser->define("m", &options->materialsCommon)
		->alias("materialsCommon")
		->defaults(false)
		->description("output materials using the KHR_materials_common extension");

	parser->define("doubleSided", &options->doubleSided)
		->defaults(false)
		->description("Force all materials to be double sided. When this value is true, back-face culling is disabled and double sided lighting is enabled");

	parser->define("v", &options->version)
		->alias("version")
		->description("glTF version to output (e.g. '1.0', '2.0')");

	parser->define("metallicRoughnessTextures", &options->metallicRoughnessTexturePaths)
		->description("paths to images to use as the PBR metallicRoughness textures");

	parser->define("specularGlossiness", &options->specularGlossiness)
		->defaults(false)
		->description("output PBR materials with the KHR_materials_pbrSpecularGlossiness extension");

	parser->define("lockOcclusionMetallicRoughness", &options->lockOcclusionMetallicRoughness)
		->defaults(false)
		->description("set metallicRoughnessTexture to be the same as the occlusionTexture in materials where an ambient texture is defined");

	parser->define("d", &options->dracoCompression)
		->alias("dracoCompression")
		->defaults(false)
		->description("compress the geometries using Draco compression extension");

	parser->define("qp", &options->positionQuantizationBits)
		->description("position quantization bits used in Draco compression extension");

	parser->define("qn", &options->normalQuantizationBits)
		->description("normal quantization bits used in Draco compression extension");

	parser->define("qt", &options->texcoordQuantizationBits)
		->description("texture coordinate quantization bits used in Draco compression extension");

	parser->define("qc", &options->colorQuantizationBits)
		->description("color quantization bits used in Draco compression extension");

	parser->define("qj", &options->jointQuantizationBits)
		->description("joint indices and weights quantization bits used in Draco compression extension");

	if (parser->parse(argc, argv)) {
		// Resolve and sanitize paths
		path inputPath = path(options->inputPath);
		options->inputPath = inputPath.string();
		options->name = inputPath.stem().string();

		path outputPath;
		if (options->outputPath == "") {
			outputPath = inputPath.parent_path() / "output" / inputPath.stem();
			outputPath += ".gltf";
		}
		else {
			outputPath = path(options->outputPath);
		}
		if (options->binary && outputPath.extension() != "glb") {
			outputPath = outputPath.parent_path() / outputPath.stem();
			outputPath += ".glb";
		}
		options->outputPath = outputPath.string();

		path basePath;
		if (options->basePath == "") {
			basePath = inputPath.parent_path();
		}
		else {
			basePath = path(options->basePath);
		}
		options->basePath = basePath.string();

		// Export flags
		if (separate != 0) {
			options->embeddedBuffers = false;
			options->embeddedShaders = false;
			options->embeddedTextures = false;
		}
		if (separateTextures != 0) {
			options->embeddedTextures = false;
		}

		if (options->version == "1.0" && !options->materialsCommon) {
			options->glsl = true;
		}

		if (options->glsl && options->materialsCommon) {
			std::cout << "ERROR: Cannot export with both glsl and materialsCommon enabled" << std::endl;
			return -1;
		}
		if ((options->glsl || options->materialsCommon) && options->specularGlossiness) {
			std::cout << "ERROR: Cannot enable specularGlossiness unless the materials are exported as PBR" << std::endl;
			return -1;
		}
		if ((options->glsl || options->materialsCommon) && options->lockOcclusionMetallicRoughness) {
			std::cout << "ERROR: Cannot enable lockOcclusionMetallicRoughness unless the materials are exported as PBR" << std::endl;
			return -1;
		}

		std::cout << "Converting " << options->inputPath << " -> " << options->outputPath << std::endl;
		std::clock_t start = std::clock();

		COLLADASaxFWL::Loader* loader = new COLLADASaxFWL::Loader();
		COLLADA2GLTF::ExtrasHandler* extrasHandler = new COLLADA2GLTF::ExtrasHandler(loader);
		COLLADA2GLTF::Writer* writer = new COLLADA2GLTF::Writer(asset, options, extrasHandler);
		loader->registerExtraDataCallbackHandler((COLLADASaxFWL::IExtraDataCallbackHandler*)extrasHandler);
		COLLADAFW::Root root(loader, writer);
		if (!root.loadDocument(options->inputPath)) {
			std::cout << "ERROR: Unable to load input from path '" << options->inputPath << "'" << std::endl;
			return -1;
		}

		glTF::Utils::writeAssetToGlTF(asset, options, &outputPath);

		std::clock_t end = std::clock();
		std::cout << "Time: " << ((end - start) / (double)(CLOCKS_PER_SEC / 1000)) << " ms" << std::endl;
		return 0;
	}
	else {
		return -1;
	}
}
