#include "Rasteriser.h"

Rasteriser app;

string Rasteriser::ModelPath()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}

bool Rasteriser::Initialise()
{
	_modelpath="";
	_modelpath = ModelPath() + "\\cube.md2";

	if (!MD2Loader::LoadModel(_modelpath.c_str(), _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}
	_model;
	
	Camera temp(0, 0, 0, Vertex(0, 0, -50, 1)); 
	_camera = temp;
	return true;
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{

	//TODO : Create the DrawWireFrame method
	// here is some help how to implement this

	// 1. Get the Polygons from the _model and store it to a local list std::vector<Polygon3D>
	std::vector<Polygon3D> polygons(_model.GetPolygons());

	// 2. Get the polygons size (How many there are)
	int numOfPolygons = polygons.size();

	// 3. Get the current transformations from the _model and store it to a local list std::vector<Vertex>
	std::vector<Vertex> transformations(_model.GetTransform());

	// 4. Create a Pen while store the current pen
	 		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);

	// 5. Make a loop for 0 till you reach the polygon size
	//    a. for each polygon get the Index of it and then the Vertex that corresponds to this index.
	//       You will have three Vertexes for each polygon
	//    b. Draw the polygon (triangle) using the LineTo function
	//		MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
	//		LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
	//		LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
	//		LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
			for (int i = 0; i < numOfPolygons; i++)
			{
				Polygon3D polygon = polygons[i];
				int index0 = polygon.GetIndex(0);
				int index1 = polygon.GetIndex(1);
				int index2 = polygon.GetIndex(2);

				Vertex number0 = transformations[index0];
				Vertex number1 = transformations[index1];
				Vertex number2 = transformations[index2];

				MoveToEx(bitmap.GetDC(), number0.GetIntX(), number0.GetIntY(), NULL);
				LineTo(bitmap.GetDC(), number1.GetIntX(), number1.GetIntY());
				LineTo(bitmap.GetDC(), number2.GetIntX(), number2.GetIntY());
				LineTo(bitmap.GetDC(), number0.GetIntX(), number0.GetIntY());
			}
}



void Rasteriser::GenerateProjectionMatrix(float d, float aspectRatio)
{
	// Perspective projection
	Matrix perspective{ d / aspectRatio, 0, 0, 0,
						0, d, 0, 0,
						0, 0, d, 0,
						0, 0, 1, 0 };
	_perspectiveTransform = perspective;
}

void Rasteriser::GenerateScreenMatrix(float d, int width, int height) // ScrenTransformation
{
	Matrix view{ float(width) / 2, 0, 0, float(width) / 2,
				 0, float(-height) / 2, 0, float(height) / 2,
				 0, 0, d / 2, d / 2,
				 0, 0, 0, 1 };
	_screenTransform = view;
}

void Rasteriser::Update(Bitmap &bitmap)  
{
	
	float aspectRatio = float(float(bitmap.GetWidth()) / float(bitmap.GetHeight()));
	GenerateProjectionMatrix(1, aspectRatio);
	GenerateScreenMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
	// Displaying the code
	if (_frameCount < 50)
	{
		_phase = RenderPhase::Start;
		_displayText = L"Wireframe";
	}
	else if (_frameCount < 150)
	{
		_phase = RenderPhase::WireframeScaleX;
		_displayText = L"Wireframe: X Axis Scale";
		_frameScaleY = 0; 
	}
	else if (_frameCount < 250)
	{
		_phase = RenderPhase::WireframeScaleY;
		_displayText = L"Wireframe: Y Axis Scale";
		_frameScaleZ = 0;
	}
	else if (_frameCount < 350)
	{
		_phase = RenderPhase::WireframeScaleZ;
		_displayText = L"Wireframe: Z Axis Scale";
		_framesRotateX = 0;
	}
	else if (_frameCount < 450)
	{
		_phase = RenderPhase::WireframeRotateX;
		_displayText = L"Wireframe: Rotater X Axis";
		_framesRotateY = 0;
	}
	else if (_frameCount < 550)
	{
		_phase = RenderPhase::WireframeRotateY;
		_displayText = L"Wireframe: Rotater Y Axis";
		_framesRotateZ = 0;
	}
	else if (_frameCount < 650)
	{
		_phase = RenderPhase::WireframeRotateZ;
		_displayText = L"Wireframe: Rotater Z Axis";
		_framesTranslationX = 0; 
	}
	else if (_frameCount < 750)
	{
		_phase = RenderPhase::WireframeTranslationX;
		_displayText = L"Wireframe: Translation on X Axis";
		_framesTranslationY = 0; 
	}
	else if (_frameCount < 850)
	{
		_phase = RenderPhase::WireframeTranslationY;
		_displayText = L"Wireframe: Translation on Y Axis";
		_framesTranslationZ = 0; 
	}
	else if (_frameCount < 950)
	{
		_phase = RenderPhase::WireframeTranslationZ;
		_displayText = L"Wireframe: Translation on Z Axis";
	}
	_frameCount++;
	_framesScaleX++;
	_frameScaleY++;
	_frameScaleZ++;
	_framesRotateX++;
	_framesRotateY++;
	_framesRotateZ++;
	_framesTranslationX++;
	_framesTranslationY++;
	_framesTranslationZ++;
}

void Rasteriser::Render(Bitmap &bitmap)   
{
	
	bitmap.Clear(RGB(0, 0, 0));
	switch (_phase)
	{
	case RenderPhase::Start:
	{
		_modelTransform = Matrix::YRotationMatrix(0);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleX:
	{
		_modelTransform = Matrix::ScalingMatrixX(_framesScaleX);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleY:
	{
		_modelTransform = Matrix::ScalingMatrixY(_frameScaleY);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleZ:
	{
		_modelTransform = Matrix::ScalingMatrixZ(_frameScaleZ);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateX:
	{
		_modelTransform = Matrix::XRotationMatrix(_framesRotateX);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateY:
	{
		_modelTransform = Matrix:: YRotationMatrix(_framesRotateY);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateZ:
	{
		_modelTransform = Matrix::ZRotationMatrix(_framesRotateZ);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslationX:
	{
		_modelTransform = Matrix::TranslationMatrix(_framesTranslationX, 0, 0);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslationY:
	{
		_modelTransform = Matrix::TranslationMatrix(0, _framesTranslationY, 0);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslationZ:
	{
		_modelTransform = Matrix::TranslationMatrix(0, 0, _framesTranslationZ);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);

		DrawWireFrame(bitmap);
		break;
	}

	default:
		break;
	}
	DrawString(bitmap, _displayText.c_str());
	
}
void Rasteriser::DrawString(const Bitmap& bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}