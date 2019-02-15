/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <SofaSimulationCommon/xml/ObjectElement.h>
#include <SofaSimulationCommon/xml/Element.h>
#include <sofa/core/ObjectFactory.h>


namespace sofa
{

namespace simulation
{

namespace xml
{

using namespace sofa::defaulttype;
using helper::Creator;

ObjectElement::ObjectElement(const std::string& name, const std::string& type, BaseElement* parent)
    : Element<core::objectmodel::BaseObject>(name, type, parent)
{
}

bool ObjectElement::init()
{
    for (BaseElement * child : *this) {
        child->init();
    }

    return initNode();
}



bool ObjectElement::initNode()
{
    core::objectmodel::BaseContext* ctx = getParent()->getObject()->toBaseContext();

    for (const auto & attribute : attributes) {
        if (replaceAttribute.find(attribute.first) != replaceAttribute.end()) {
            setAttribute(attribute.first,replaceAttribute[attribute.first]);
        }
    }

    core::objectmodel::BaseObject::SPtr obj = core::ObjectFactory::CreateObject(ctx, this);

    if (obj == nullptr)
        obj = Factory::CreateObject(this->getType(), this);

    if (obj == nullptr) {
        BaseObjectDescription desc("InfoComponent", "InfoComponent") ;
        desc.setAttribute("name", ("Not created ("+getType()+")"));
        obj = core::ObjectFactory::CreateObject(ctx, &desc) ;
        std::stringstream tmp ;
        for(const auto & s : this->getErrors())
            tmp << s << msgendl ;

        if(obj) {
           obj->init() ;
           msg_error(obj.get()) << tmp.str() ;
           return false;
        }

        msg_error(ctx) << tmp.str() ;
        return false;
    }

    setObject(obj);

    /// display any unused attributes
    for (const auto & attribute : attributes) {
        if (not attribute.second.isAccessed())
        {
            std::string name = attribute.first;

            /// ignore some prefix that are used to quickly disable parameters in XML files
            if (name.substr(0,1) == "_" || name.substr(0,2) == "NO")
                continue;

            msg_warning(obj.get())
            << SOFA_FILE_INFO_COPIED_FROM(getSrcFile(), getSrcLine())
            << "Unused Attribute: \""<<attribute.first <<"\" with value: \"" <<attribute.second.c_str() <<"\"" ;        }
    }
    return true;
}

Creator<BaseElement::NodeFactory, ObjectElement> ObjectNodeClass("Object");

const char* ObjectElement::getClass() const
{
    return ObjectNodeClass.c_str();
}

} // namespace xml

} // namespace simulation

} // namespace sofa

