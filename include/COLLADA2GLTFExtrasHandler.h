#pragma once

#include <set>

#include "GeneratedSaxParser.h"
#include "COLLADASaxFWLLoader.h"
#include "COLLADASaxFWLIExtraDataCallbackHandler.h"

namespace COLLADA2GLTF {
	class ExtrasHandler : COLLADASaxFWL::IExtraDataCallbackHandler {
	private:
		virtual bool elementBegin(const COLLADASaxFWL::ParserChar* elementName, const GeneratedSaxParser::xmlChar** attributes);
		virtual bool elementEnd(const COLLADASaxFWL::ParserChar* elementName);
		virtual bool textData(const COLLADASaxFWL::ParserChar* text, size_t textLength);

		virtual bool parseElement(
			const COLLADASaxFWL::ParserChar* profileName,
			const COLLADASaxFWL::StringHash& elementHash,
			const COLLADAFW::UniqueId& uniqueId,
			COLLADAFW::Object* object);

		std::shared_ptr<COLLADASaxFWL::Loader> _loader;
		COLLADAFW::UniqueId _currentId;
		bool _inBump = false;
		bool _inDoubleSided = false;
	public:
		std::set<COLLADAFW::UniqueId> lockAmbientDiffuse;
		std::shared_ptr<COLLADAFW::TextureAttributes> bumpTexture;
		std::set<COLLADAFW::UniqueId> doubleSided;
		ExtrasHandler(std::shared_ptr<COLLADASaxFWL::Loader> loader) : _loader(loader) {};
	};
}
