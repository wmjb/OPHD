// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "StructureInspector.h"

#include "../Cache.h"
#include "../Constants.h"
#include "../Things/Structures/Structure.h"
#include "StringTable.h"
#include "TextRender.h"

#include <NAS2D/Utility.h>

#include <map>
#include <sstream>
#include <stdexcept>


using namespace NAS2D;

static const Font* FONT = nullptr;
static const Font* FONT_BOLD = nullptr;


StructureInspector::StructureInspector() :
	btnClose{"Close"},
	mIcons{imageCache.load("ui/icons.png")}
{
	text(constants::WINDOW_STRUCTURE_INSPECTOR);
	init();
}


/**
 * 
 */
StructureInspector::~StructureInspector()
{}


/**
 * 
 */
void StructureInspector::init()
{
	size({350, 200});

	add(&btnClose, 295, 175);
	btnClose.size({50, 20});
	btnClose.click().connect(this, &StructureInspector::btnCloseClicked);


	add(&txtStateDescription, 190, 75);
	txtStateDescription.size({155, 80});
	txtStateDescription.font(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL);

	FONT = &fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL);
	FONT_BOLD = &fontCache.load(constants::FONT_PRIMARY_BOLD, constants::FONT_PRIMARY_NORMAL);
}


/**
 * 
 */
void StructureInspector::structure(Structure* s)
{
	mStructure = s;
	check();
}


/**
 * Forces a state check. Used to update text area for descriptions.
 */
void StructureInspector::check()
{
	if (!mStructure) { return; }

	txtStateDescription.text("");

	if (mStructure->disabled() || mStructure->destroyed()) { txtStateDescription.text(disabledReason(mStructure->disabledReason())); }
	else if (mStructure->isIdle()) { txtStateDescription.text(idleReason(mStructure->idleReason())); }

	txtStateDescription.visible(!txtStateDescription.text().empty());
}


/**
 * 
 */
void StructureInspector::btnCloseClicked()
{
	visible(false);
}


void StructureInspector::drawPopulationRequirements()
{
	auto& renderer = Utility<Renderer>::get();

	auto position = mRect.startPoint() + NAS2D::Vector{10, 85};
	renderer.drawText(*FONT_BOLD, "Population Required", position, NAS2D::Color::White);

	const std::array<std::string, 2> populationTypes{
		"Workers",
		"Scientists"
	};

	position.y += 20;
	for (std::size_t populationType = 0; populationType < populationTypes.size(); ++populationType) {
		const auto& populationRequirements = mStructure->populationRequirements();
		const auto& populationAvailable = mStructure->populationAvailable();
		if (populationRequirements[populationType] > 0)
		{
			std::string text = populationTypes[populationType] + ": " + std::to_string(populationAvailable[populationType]) + "/" + std::to_string(populationRequirements[populationType]);
			Color color = populationAvailable[populationType] >= populationRequirements[populationType] ? Color::White : Color::Red;
			renderer.drawText(*FONT, text, position, color);
			position.y += 10;
		}
	}
}


/**
 * 
 */
void StructureInspector::update()
{
	if (!visible()) { return; }
	Window::update();

	auto& renderer = Utility<Renderer>::get();

	if (mStructure == nullptr)
	{
		throw std::runtime_error("Null pointer to structure within StructureInspector");
	}
	text(mStructure->name());

	auto position = mRect.startPoint() + NAS2D::Vector{ 5, 25 };
	drawLabelAndValue(position,"Type: ", mStructure->classDescription());

	position.y += 20;
	drawLabelAndValue(position, "Power Required: ", std::to_string(mStructure->energyRequirement()));

	position = mRect.startPoint() + NAS2D::Vector{190, 25};
	drawLabelAndValue(position,"State: ", mStructure->stateDescription());

	position.y += 20;
	if (mStructure->underConstruction())
	{
		drawLabelAndValue(position,"Turns Remaining: ", std::to_string(mStructure->turnsToBuild() - mStructure->age()));
	}
	else
	{
		drawLabelAndValue(position,"Age: ", std::to_string(mStructure->age()) + " of " + std::to_string(mStructure->maxAge()));
	}

	drawPopulationRequirements();

	StringTable stringTable = mStructure->createInspectorViewTable();
	stringTable.computeRelativeCellPositions();
	stringTable.position(mRect.startPoint() + NAS2D::Vector<float>{10, 135});
	stringTable.draw(renderer);
}
