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
#include <SofaMeshCollision/BarycentricPenalityContact.inl>
#include <SofaMeshCollision/BarycentricContactMapper.h>
#include <SofaMeshCollision/BarycentricContactMapper.inl>

#include <SofaMiscCollision/TetrahedronModel.h>

namespace sofa
{

namespace component
{

namespace collision
{

using namespace defaulttype;
using simulation::Node;
using namespace sofa::core::collision;

Creator<Contact::Factory, BarycentricPenalityContact<TetrahedronCollisionModel, SphereCollisionModel<sofa::defaulttype::Vec3Types>> > TetrahedronSpherePenalityContactClass("default",true);
Creator<Contact::Factory, BarycentricPenalityContact<TetrahedronCollisionModel, PointCollisionModel<sofa::defaulttype::Vec3Types>> > TetrahedronPointPenalityContactClass("default",true);
Creator<Contact::Factory, BarycentricPenalityContact<TetrahedronCollisionModel, LineCollisionModel<sofa::defaulttype::Vec3Types>> > TetrahedronLinePenalityContactClass("default",true);
Creator<Contact::Factory, BarycentricPenalityContact<TetrahedronCollisionModel, TriangleCollisionModel<sofa::defaulttype::Vec3Types>> > TetrahedronTrianglePenalityContactClass("default",true);
Creator<Contact::Factory, BarycentricPenalityContact<TetrahedronCollisionModel, TetrahedronCollisionModel> > TetrahedronTetrahedronPenalityContactClass("default",true);

} // namespace collision

} // namespace component

} // namespace sofa
