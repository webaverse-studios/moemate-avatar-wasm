#include "PxPhysicsVersion.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "PxSceneDesc.h"
#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxMaterial.h"
#include "extensions/PxDefaultStreams.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSphericalJoint.h"
#include "extensions/PxD6Joint.h"
#include "geometry/PxTriangleMeshGeometry.h"
#include "cooking/PxTriangleMeshDesc.h"
#include "cooking/PxCooking.h"
#include "extensions/PxTriangleMeshExt.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidBodyExt.h"
#include "extensions/PxRigidActorExt.h"
// #include "PxPhysicsAPI.h"
#include "PxQueryReport.h"
#include "PxSimulationEventCallback.h"
#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxCapsuleController.h"
#include "geometry/PxGeometryQuery.h"