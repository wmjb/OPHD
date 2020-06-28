#pragma once

#include "Structure.h"
#include "../../UI/TextRender.h"


/**
 * \class	Residence
 * \brief	Base Residential structure.
 * 
 * Implements the base Residence structures. Upgraded residences should derive
 * from this class and override the definition of mCapacity.
 */
class Residence : public Structure
{
public:
	Residence() : Structure(constants::RESIDENCE, "structures/residential_1.sprite", StructureClass::CLASS_RESIDENCE)
	{
		sprite().play(constants::STRUCTURE_STATE_CONSTRUCTION);
		maxAge(500);
		turnsToBuild(2);

		requiresCHAP(true);
	}

	int capacity() const { return mCapacity; }

	void drawInspectorView([[maybe_unused]] NAS2D::Renderer& renderer, const NAS2D::Rectangle<float>& windowRect) override
	{
		auto position = windowRect.startPoint() + NAS2D::Vector{ 10, 135 };
		drawLabelAndValue(position, "Colonist Capacity: ", std::to_string(capacity()));
	}

protected:
	void defineResourceInput() override
	{
		resourcesIn().energy(2);
	}

protected:
	int mCapacity = 25; /**< Override this value in derived residences.*/
};
