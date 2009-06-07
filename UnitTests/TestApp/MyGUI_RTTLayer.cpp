/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_FactoryManager.h"

namespace MyGUI
{

	RTTLayer::RTTLayer(xml::ElementPtr _node, Version _version) :
		OverlappedLayer(_node->findAttribute("name"), utility::parseBool(_version < Version(1, 0) ? _node->findAttribute("peek") : _node->findAttribute("pick"))),
		mData(nullptr)
	{
		mVersion = _version;
		mData = _node->createCopy();
	}

	RTTLayer::~RTTLayer()
	{
		delete mData;
	}

	ILayerNode * RTTLayer::createChildItemNode()
	{
		// ������� ������� �����
		RTTLayerNode* node = new RTTLayerNode(this);
		mChildItems.push_back(node);

		if (mData != nullptr)
		{
			FactoryManager& factory = FactoryManager::getInstance();

			MyGUI::xml::ElementEnumerator controller = mData->getElementEnumerator();
			while (controller.next())
			{
				Object* object = factory.createObject(controller->getName(), controller->findAttribute("type"));
				if (object == nullptr) continue;

				LayerNodeAnimation* data = object->castType<LayerNodeAnimation>(false);
				if (data == nullptr)
				{
					factory.destroyObject(object);
					continue;
				}
				data->deserialization(controller.current(), mVersion);
				node->setLayerNodeAnimation(data);
			}
		}

		return node;
	}

} // namespace MyGUI
