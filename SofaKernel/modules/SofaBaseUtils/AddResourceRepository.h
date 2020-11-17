/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
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
#ifndef SOFA_COMPONENT_MISC_ADDRESOURCEREPOSITORY_H
#define SOFA_COMPONENT_MISC_ADDRESOURCEREPOSITORY_H

#include <SofaBaseUtils/config.h>

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/helper/system/FileRepository.h>
#include <sofa/helper/system/PluginManager.h>

namespace sofa
{
namespace component
{
namespace misc
{


class SOFA_BASE_UTILS_API BaseAddResourceRepository: public sofa::core::objectmodel::BaseObject
{
public:
    SOFA_ABSTRACT_CLASS(BaseAddResourceRepository, sofa::core::objectmodel::BaseObject);

protected:
    BaseAddResourceRepository();
    ~BaseAddResourceRepository() override;

    sofa::helper::system::FileRepository* m_repository;

public:
    sofa::core::objectmodel::DataFileName d_repositoryPath; ///< Path to add to the pool of resources

    void parse(sofa::core::objectmodel::BaseObjectDescription* arg) override;
    bool updateRepositoryPath();
    void cleanup() override;

private:
    std::string m_currentAddedPath;

    virtual sofa::helper::system::FileRepository* getFileRepository() = 0;
};


/// Add a new path to DataRepository
class SOFA_BASE_UTILS_API AddDataRepository: public BaseAddResourceRepository
{
public:
    SOFA_CLASS(AddDataRepository, BaseAddResourceRepository);

protected:
    sofa::helper::system::FileRepository* getFileRepository() override { return &sofa::helper::system::DataRepository; }
};


/// Add a new path to PluginRepository
class SOFA_BASE_UTILS_API AddPluginRepository: public BaseAddResourceRepository
{
public:
    SOFA_CLASS(AddPluginRepository, BaseAddResourceRepository);

protected:
    sofa::helper::system::FileRepository* getFileRepository() override {
        using sofa::helper::system::PluginManager;
        return &PluginManager::getInstance().getPluginRepository();
    }
};


} // namespace misc
} // namespace component
} // namespace sofa

#endif // SOFA_COMPONENT_MISC_ADDRESOURCEREPOSITORY_H
