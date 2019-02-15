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
#include "BaseObjectDescription.h"
#include "BaseContext.h"
#include "BaseObject.h"
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <iostream>
#include <sofa/helper/logging/Messaging.h>
#include <sofa/helper/system/Locale.h>

namespace sofa
{

namespace core
{

namespace objectmodel
{

BaseObjectDescription::BaseObjectDescription(const std::string & name, const std::string &  type)
{
    setAttribute("name", name);
    setAttribute("type", type);
}

/// Get the associated object (or NULL if it is not created yet)
Base* BaseObjectDescription::getObject()
{
    return nullptr;
}

/// Get the associated object (or NULL if it is not created yet)
const Base * BaseObjectDescription::getObject() const
{
    return nullptr;
}

/// Get the object instance name
std::string BaseObjectDescription::getName() const noexcept
{
    return std::string(getAttribute("name",""));
}

void BaseObjectDescription::setName(const std::string& name)
{
    setAttribute("name",name);
}

/// Get the parent node
BaseObjectDescription* BaseObjectDescription::getParent()
{
    return nullptr;
}

/// Get the parent node
const BaseObjectDescription * BaseObjectDescription::getParent() const
{
    return nullptr;
}

/// Get the file where this description was read from. Useful to resolve relative file paths.
std::string BaseObjectDescription::getBaseFile() const noexcept
{
    return "";
}

///// Get all attribute data, read-only
const BaseObjectDescription::AttributeMap& BaseObjectDescription::getAttributeMap() const noexcept
{
    return attributes;
}

/// Find an object description given its name (relative to this object)
BaseObjectDescription* BaseObjectDescription::find(const char* /*nodeName*/, bool /*absolute*/)
{
    return nullptr;
}

/// Remove an attribute given its name, returns false if the attribute was not there.
bool BaseObjectDescription::removeAttribute(const std::string& attr)
{
    auto it = attributes.find(attr);
    if (it == attributes.end())
        return false;

    attributes.erase(it);
    return true;
}

/// Get an attribute given its name (return defaultValue if not present)
std::string BaseObjectDescription::getAttribute(const std::string & name, const std::string & defaultValue) const noexcept
{
    if (hasAttribute(name))
        return attributes.at(name);
    else
        return defaultValue;
}

/// Get an attribute given its name
/// @throws std::out_of_range exception when the attribute's name doesn't exist.
std::string BaseObjectDescription::getAttribute(const std::string & name) const
{
    if (hasAttribute(name))
        return attributes.at(name);
    else
        throw std::out_of_range("Trying to access an attribute named '" + name +
                                "', but such attribute does not exist in the current object '" + getName() + "'");
}

/// Docs is in .h
float BaseObjectDescription::getAttributeAsFloat(const std::string& attr, const float defaultVal)
{
    auto it = attributes.find(attr);
    if (it == attributes.end())
        return defaultVal;

    // Make sure that strtof uses a dot '.' as the decimal separator.
    helper::system::TemporaryLocale locale(LC_NUMERIC, "C");

    const char* attrstr=it->second.c_str();
    char* end=nullptr;
    float retval = strtof(attrstr, &end);

    /// It is important to check that the attribute was totally parsed to report
    /// message to users because a silent error is the worse thing that can happen in UX.
    if(end !=  attrstr+strlen(attrstr)){
        std::stringstream msg;
        msg << "Unable to parse a float value from attribute '" << attr << "'='"<<it->second.c_str()<<"'. "
               "Use the default value '"<<defaultVal<< "' instead.";
        errors.push_back(msg.str());
        return defaultVal ;
    }

    return retval ;
}

/// Docs is in .h
long int BaseObjectDescription::getAttributeAsInt(const std::string& attr, const long int defaultVal)
{
    auto it = attributes.find(attr);
    if (it == attributes.end())
        return defaultVal;

    const char* attrstr=it->second.c_str();
    char* end=nullptr;
    auto retval = strtol(attrstr, &end, 10);

    /// It is important to check that the attribute was totally parsed to report
    /// message to users because a silent error is the worse thing that can happen in UX.
    if(end !=  attrstr+strlen(attrstr)){
        std::stringstream msg;
        msg << "Unable to parse an integer value from attribute '" << attr << "'='"<<it->second.c_str()<<"'. "
               "Use the default value '"<<defaultVal<< "' instead.";
        errors.push_back(msg.str());
        return defaultVal;
    }

    return retval ;
}

bool BaseObjectDescription::hasAttribute(const std::string &name) const noexcept
{
    return (attributes.find(name) != attributes.end());
}

void BaseObjectDescription::setAttribute(const std::string& attr, const char* val)
{
    std::string s = val;
    return setAttribute(attr, s);
}


/// Set an attribute. Override any existing value
void BaseObjectDescription::setAttribute(const std::string& attr, const std::string &val)
{
    attributes[attr] = val;
}

std::string BaseObjectDescription::getFullName() const
{
    const BaseObjectDescription * parent = getParent();

    if (parent == nullptr)
        return "/";

    return parent->getFullName() + "/" + getName();
}

/// Find an object given its name
Base* BaseObjectDescription::findObject(const char* nodeName)
{
    BaseObjectDescription* node = find(nodeName);
    if (node != nullptr)
    {
        Base* obj = node->getObject();
        BaseContext* ctx = obj->toBaseContext();

        if (ctx != nullptr)
            obj = ctx->getMechanicalState();

        return obj;
    }
    else
    {
        msg_error("BaseObjectDescription") << "findObject: Node "<<nodeName<<" NOT FOUND.";
        return nullptr;
    }
}

} // namespace objectmodel

} // namespace core

} // namespace sofa
