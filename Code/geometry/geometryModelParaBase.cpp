#include "geometryModelParaBase.h"
#include "geometryData.h"
#include "geometryParaBox.h"
#include "geometryParaFillet.h"
#include "geometryParaCone.h"
#include "geometryParaCylinder.h"
#include "geometryParaSphere.h"
#include "geometryParaPoint.h"
#include "geometryParaLine.h"
#include "geometryParaFace.h"
#include "geometryParaChamfer.h"
#include "geometryParaVariableFillet.h"
#include "geometryParaBoolOperation.h"
#include "geometryParaMakeMirror.h"
#include "geometryParaMakeMove.h"
#include "geometryParaRotateFeature.h"
#include "geometryParaMakeMatrix.h"
#include "geometryParaExtrusion.h"
#include "geometryParaLoft.h"
#include "geometryParaSweep.h"
#include "geometryParaRevol.h"

namespace Geometry
{

	GeometryModelParaBase::GeometryModelParaBase()
	{
		_geoData = GeometryData::getInstance();
	}


	Geometry::GeometryParaType GeometryModelParaBase::getParaType()
	{
		return _type;
	}

	QString GeometryModelParaBase::typeToString()
	{
		QString stype;

		switch (_type)
		{
			case GeometryParaNone: stype = "None"; break;
			case GeometryParaCreateBox: stype = "CreateBox"; break;
			case GeometryParaCreateCylinder:stype = "CreateCylinder"; break;
			case GeometryParaCreateCone:stype = "CreateCone"; break;
			case GeometryParaCreateFillet: stype = "CreateFillet"; break;
			case GeometryParaCreateSphere: stype = "CreateSphere"; break;
			case GeometryParaCreatePoint: stype = "CreatePoint"; break;
			case GeometryParaCreateLine: stype = "CreateLine"; break;
			case GeometryParaCreateFace: stype = "CreateFace"; break;
			case GeometryParaMakeChamfer: stype = "MakeChamfer"; break;
			case GeometryParaMakeVariableFillet: stype = "MakeVariableFillet"; break;
			case GeometryParaMakeBoolOperation: stype = "MakeBoolOperation"; break;
			case GeometryParaMakeMirrorFeature:stype = "MakeMirrorFeature"; break;
			case GeometryParaMakeRotateFeature: stype = "MakeRotateFeature"; break;
			case GeometryParaMakeMoveFeature: stype = "MakeMoveFeature"; break;
			case GeometryParaMakeMatrix:stype = "MakeMatrix"; break;
			case GeometryParaMakeExtrusion:stype = "MakeExtrusion"; break;
			case GeometryParaMakeLoft:stype = "MakeLoft"; break;
			case GeometryParaMakeSweep:stype = "MakeSweep"; break;
			case GeometryParaMakeRevol:stype = "MakeRevol"; break;
			default: 
				break;

		}
	
		return stype;
	}

	GeometryModelParaBase* GeometryModelParaBase::createParaByString(QString s)
	{
		GeometryModelParaBase* p = nullptr;

		if (s == "CreateBox") p = new GeometryParaBox;
		else if (s == "CreateCylinder") p = new GeometryParaCylinder;
		else if (s == "CreateCone") p = new GeometryParaCone;
		else if (s == "CreateFillet") p = new GeometryParaFillet;
		else if (s == "CreateSphere") p = new GeometryParaSphere;
		else if (s == "CreatePoint") p = new GeometryParaPoint;
		else if (s == "CreateLine") p = new GeometryParaLine;
		else if (s == "CreateFace") p = new GeometryParaFace;
		else if (s == "MakeChamfer") p = new GeometryParaChamfer;
		else if (s == "MakeVariableFillet") p = new GeometryParaVariableFillet;
		else if (s == "MakeBoolOperation") p = new GeometryParaBoolOperation;
		else if (s == "MakeMirrorFeature") p = new GeometryParaMirrorFeature;
		else if (s == "MakeRotateFeature") p = new GeometryParaRotateFeature;
		else if (s == "MakeMoveFeature") p = new GeometryParaMakeMove;
		else if (s == "MakeMatrix") p = new GeometryParaMatrix;
		else if (s == "MakeExtrusion") p = new GeometryParaExtrusion;
		else if (s == "MakeLoft") p = new GeometryParaLoft;
		else if (s == "MakeSweep") p = new GeometryParaSweep;
		else if (s == "MakeRevol") p = new GeometryParaRevol;

		return p;
	}


}