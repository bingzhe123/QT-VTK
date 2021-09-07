#include "MyVtkAxisActor2D.h"
#include "vtkAxisActor2D.h"

#include "vtkCellArray.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkViewport.h"
#include "vtkWindow.h"
#include "vtkMath.h"

#include <cmath>
#include <limits>
MyVtkAxisActor2D::~MyVtkAxisActor2D()
{
}

MyVtkAxisActor2D * MyVtkAxisActor2D::New()
{
	return new MyVtkAxisActor2D();
}

MyVtkAxisActor2D::MyVtkAxisActor2D()
{
		NumberOfLabels = 2;
}

void MyVtkAxisActor2D::BuildAxis(vtkViewport * viewport)
{
	int i, *x, viewportSizeHasChanged, positionsHaveChanged;
	vtkIdType ptIds[2];
	double p1[3], p2[3], offset;
	double interval, deltaX, deltaY;
	double xTick[3];
	double theta, val;
	int *size, stringSize[2];
	char string[512];

	if (this->TitleVisibility && !this->TitleTextProperty)
	{
		vtkErrorMacro(<< "Need title text property to render axis actor");
		return;
	}

	if (this->LabelVisibility && !this->LabelTextProperty)
	{
		vtkErrorMacro(<< "Need label text property to render axis actor");
		return;
	}

	// Check to see whether we have to rebuild everything
	// Viewport change may not require rebuild
	positionsHaveChanged = 0;
	int *lastPosition =
		this->PositionCoordinate->GetComputedViewportValue(viewport);
	int *lastPosition2 =
		this->Position2Coordinate->GetComputedViewportValue(viewport);
	if (lastPosition[0] != this->LastPosition[0] ||
		lastPosition[1] != this->LastPosition[1] ||
		lastPosition2[0] != this->LastPosition2[0] ||
		lastPosition2[1] != this->LastPosition2[1])
	{
		positionsHaveChanged = 1;
	}

	// See whether fonts have to be rebuilt (font size depends on viewport size)
	viewportSizeHasChanged = 0;
	size = viewport->GetSize();
	if (this->LastSize[0] != size[0] || this->LastSize[1] != size[1])
	{
		viewportSizeHasChanged = 1;
		this->LastSize[0] = size[0];
		this->LastSize[1] = size[1];
	}

	if (!viewport->GetVTKWindow() ||
		(!positionsHaveChanged && !viewportSizeHasChanged &&
			viewport->GetMTime() < this->BuildTime &&
			viewport->GetVTKWindow()->GetMTime() < this->BuildTime &&
			this->GetMTime() < this->BuildTime &&
			(!this->LabelVisibility ||
				this->LabelTextProperty->GetMTime() < this->BuildTime) &&
				(!this->TitleVisibility ||
					this->TitleTextProperty->GetMTime() < this->BuildTime)))
	{
		return;
	}

	vtkDebugMacro(<< "Rebuilding axis");

	// Initialize and get important info
	this->Axis->Initialize();
	this->AxisActor->SetProperty(this->GetProperty());

	// Compute the location of tick marks and labels

	this->UpdateAdjustedRange();

	interval = (this->AdjustedRange[1] - this->AdjustedRange[0]) / (this->AdjustedNumberOfLabels - 1);

	this->NumberOfLabelsBuilt = this->AdjustedNumberOfLabels;

	// Generate the axis and tick marks.
	// We'll do our computation in viewport coordinates. First determine the
	// location of the endpoints.
	x = this->PositionCoordinate->GetComputedViewportValue(viewport);
	p1[0] = x[0];
	p1[1] = x[1];
	p1[2] = 0.0;
	this->LastPosition[0] = x[0];
	this->LastPosition[1] = x[1];

	x = this->Position2Coordinate->GetComputedViewportValue(viewport);
	p2[0] = x[0];
	p2[1] = x[1];
	p2[2] = 0.0;
	this->LastPosition2[0] = x[0];
	this->LastPosition2[1] = x[1];

	double *xp1, *xp2, len = 0.0;
	if (this->SizeFontRelativeToAxis)
	{
		xp1 = this->PositionCoordinate->GetComputedDoubleViewportValue(viewport);
		xp2 = this->Position2Coordinate->GetComputedDoubleViewportValue(viewport);
		len = sqrt((xp2[0] - xp1[0])*(xp2[0] - xp1[0]) + (xp2[1] - xp1[1])*(xp2[1] - xp1[1]));
	}

	vtkPoints *pts = vtkPoints::New();
	vtkCellArray *lines = vtkCellArray::New();
	this->Axis->SetPoints(pts);
	this->Axis->SetLines(lines);
	pts->Delete();
	lines->Delete();

	// Generate point along axis (as well as tick points)
	deltaX = p2[0] - p1[0];
	deltaY = p2[1] - p1[1];

	if (deltaX == 0. && deltaY == 0.)
	{
		theta = 0.;
	}
	else
	{
		theta = atan2(deltaY, deltaX);
	}

	// First axis point, where first tick is located
	ptIds[0] = pts->InsertNextPoint(p1);
	xTick[0] = p1[0] + this->TickLength*sin(theta);
	xTick[1] = p1[1] - this->TickLength*cos(theta);
	xTick[2] = 0.0;
	pts->InsertNextPoint(xTick);

	// Set up creation of ticks
	double p21[3], length;
	p21[0] = p2[0] - p1[0];
	p21[1] = p2[1] - p1[1];
	p21[2] = p2[2] - p1[2];
	length = vtkMath::Normalize(p21);

	// Sum of all the ticks: minor and majors. Contains the start and end ticks.
	int numTicks;
	// Distance between each minor tick.
	double distance;
	if (this->RulerMode)
	{
		double wp1[3], wp2[3], wp21[3];
		this->PositionCoordinate->GetValue(wp1);
		this->Position2Coordinate->GetValue(wp2);
		wp21[0] = wp2[0] - wp1[0];
		wp21[1] = wp2[1] - wp1[1];
		wp21[2] = wp2[2] - wp1[2];
		const double worldLength = vtkMath::Norm(wp21);
		const double worldDistance = this->RulerDistance / (this->NumberOfMinorTicks + 1);
		numTicks = static_cast<int>(
			worldDistance <= 0.0 ? 0.0 : (worldLength / worldDistance));
		const double precision = std::numeric_limits<double>::epsilon();
		const bool hasRemainderInDivision = worldDistance <= 0.0 ? false :
			std::fmod(worldLength, worldDistance) > precision;
		// numTicks must contain the start and end ticks
		// Don't add the end tick if it is already in numTicks:
		//   when wLength / wDistance is an integer.
		numTicks += hasRemainderInDivision ? 2 : 1;
		// Tick distance was computed in world coordinates, convert to viewport
		// coordinates.
		const double worldToLocalRatio =
			(worldLength <= 0.0 ? 0.0 : length / worldLength);
		distance = worldDistance * worldToLocalRatio;
	}
	else
	{
		numTicks = (this->AdjustedNumberOfLabels - 1) *
			(this->NumberOfMinorTicks + 1) + 1;
		distance = length / (numTicks - 1);
	}

	// Only draw the inner ticks (not the start/end ticks)
	for (i = 1; i < numTicks - 1; i++)
	{
		int tickLength = 0;
		if (i % (this->NumberOfMinorTicks + 1) == 0)
		{
			tickLength = this->TickLength;
		}
		else
		{
			tickLength = this->MinorTickLength;
		}
		xTick[0] = p1[0] + i * p21[0] * distance;
		xTick[1] = p1[1] + i * p21[1] * distance;
		pts->InsertNextPoint(xTick);
		xTick[0] = xTick[0] + tickLength * sin(theta);
		xTick[1] = xTick[1] - tickLength * cos(theta);
		pts->InsertNextPoint(xTick);
	}

	// Last axis point
	ptIds[1] = pts->InsertNextPoint(p2);
	xTick[0] = p2[0] + this->TickLength*sin(theta);
	xTick[1] = p2[1] - this->TickLength*cos(theta);
	pts->InsertNextPoint(xTick);

	// Add the axis if requested
	if (this->AxisVisibility)
	{
		lines->InsertNextCell(2, ptIds);
	}

	// Create lines representing the tick marks
	if (this->TickVisibility)
	{
		for (i = 0; i < numTicks; i++)
		{
			ptIds[0] = 2 * i;
			ptIds[1] = 2 * i + 1;
			lines->InsertNextCell(2, ptIds);
		}
	}

	// Build the labels
	if (this->LabelVisibility)
	{
		// Update the labels text. Do it only if the range has been adjusted,
		// i.e. if we think that new labels must be created.
		// WARNING: if LabelFormat has changed, they should be recreated too
		// but at this point the check on LabelFormat is "included" in
		// UpdateAdjustedRange(), which is the function that update
		// AdjustedRangeBuildTime or not.
		vtkMTimeType labeltime = this->AdjustedRangeBuildTime;
		if (this->AdjustedRangeBuildTime > this->BuildTime)
		{
			for (i = 0; i < this->AdjustedNumberOfLabels; i++)
			{
				val = this->AdjustedRange[0] + i * interval;
				snprintf(string, sizeof(string), this->LabelFormat, val);
				this->LabelMappers[i]->SetInput(string);

				// Check if the label text has changed

				if (this->LabelMappers[i]->GetMTime() > labeltime)
				{
					labeltime = this->LabelMappers[i]->GetMTime();
				}
			}
		}

		// Copy prop and text prop eventually
		for (i = 0; i < this->AdjustedNumberOfLabels; i++)
		{
			if (this->LabelTextProperty->GetMTime() > this->BuildTime ||
				this->AdjustedRangeBuildTime > this->BuildTime)
			{
				// Shallow copy here so that the size of the label prop is not
				// affected by the automatic adjustment of its text mapper's
				// size (i.e. its mapper's text property is identical except
				// for the font size which will be modified later). This
				// allows text actors to share the same text property, and in
				// that case specifically allows the title and label text prop
				// to be the same.
				this->LabelMappers[i]->GetTextProperty()->ShallowCopy(
					this->LabelTextProperty);
			}
		}

		// Resize the mappers if needed (i.e. viewport has changed, than
		// font size should be changed, or label text property has changed,
		// or some of the labels have changed (got bigger for example)
		if (positionsHaveChanged || viewportSizeHasChanged ||
			this->LabelTextProperty->GetMTime() > this->BuildTime ||
			labeltime > this->BuildTime)
		{
			if (!this->SizeFontRelativeToAxis)
			{
				vtkTextMapper::SetMultipleRelativeFontSize(viewport,
					this->LabelMappers,
					this->AdjustedNumberOfLabels,
					size,
					this->LastMaxLabelSize,
					0.015*this->FontFactor*this->LabelFactor);
			}
			else
			{
				int minFontSize = 1000, fontSize, minLabel = 0;
				for (i = 0; i < this->AdjustedNumberOfLabels; i++)
				{
					fontSize = this->LabelMappers[i]->
						SetConstrainedFontSize(viewport,
							static_cast<int>((1.0 / this->AdjustedNumberOfLabels)*len),
							static_cast<int>(0.2*len));
					if (fontSize < minFontSize)
					{
						minFontSize = fontSize;
						minLabel = i;
					}
				}
				for (i = 0; i < this->AdjustedNumberOfLabels; i++)
				{
					this->LabelMappers[i]->GetTextProperty()->SetFontSize(minFontSize);
				}
				this->LabelMappers[minLabel]->GetSize(viewport, this->LastMaxLabelSize);
			}
			if (!m_bValidValue)
			{
				LabelMappers[AdjustedNumberOfLabels - 1]->SetInput("* mm");
			}
		}

		// Position the mappers
		for (i = 0; i < this->AdjustedNumberOfLabels; i++)
		{
			pts->GetPoint((this->NumberOfMinorTicks + 1) * 2 * i + 1, xTick);
			this->LabelMappers[i]->GetSize(viewport, stringSize);
			this->SetOffsetPosition(xTick,
				theta,
				this->LastMaxLabelSize[0],
				this->LastMaxLabelSize[1],
				this->TickOffset,
				this->LabelActors[i]);
		}
	} // If labels visible

	// Now build the title
	if (this->Title != nullptr && this->Title[0] != 0 && this->TitleVisibility)
	{
		this->TitleMapper->SetInput(this->Title);

		if (this->TitleTextProperty->GetMTime() > this->BuildTime)
		{
			// Shallow copy here so that the size of the title prop is not
			// affected by the automatic adjustment of its text mapper's
			// size (i.e. its mapper's text property is identical except for
			// the font size which will be modified later). This allows text
			// actors to share the same text property, and in that case
			// specifically allows the title and label text prop to be the same.
			this->TitleMapper->GetTextProperty()->ShallowCopy(
				this->TitleTextProperty);
		}

		if (positionsHaveChanged || viewportSizeHasChanged ||
			this->TitleTextProperty->GetMTime() > this->BuildTime)
		{
			if (!this->UseFontSizeFromProperty)
			{
				if (!this->SizeFontRelativeToAxis)
				{
					vtkTextMapper::SetRelativeFontSize(this->TitleMapper, viewport, size, stringSize, 0.015*this->FontFactor);
				}
				else
				{
					this->TitleMapper->SetConstrainedFontSize(viewport,
						static_cast<int>(0.33*len),
						static_cast<int>(0.2*len));
					this->TitleMapper->GetSize(viewport, stringSize);
				}
			}
			else
			{
				this->TitleMapper->GetSize(viewport, stringSize);
			}
		}
		else
		{
			this->TitleMapper->GetSize(viewport, stringSize);
		}

		xTick[0] = p1[0] + (p2[0] - p1[0]) * this->TitlePosition;
		xTick[1] = p1[1] + (p2[1] - p1[1]) * this->TitlePosition;
		xTick[0] = xTick[0] + (this->TickLength + this->TickOffset) * sin(theta);
		xTick[1] = xTick[1] - (this->TickLength + this->TickOffset) * cos(theta);

		offset = 0.0;
		if (this->LabelVisibility)
		{
			offset = this->ComputeStringOffset(this->LastMaxLabelSize[0],
				this->LastMaxLabelSize[1],
				theta);
		}

		{
			double x, y, center[2];
			int pos[2];

			x = stringSize[0] / 2.0 + offset;
			y = stringSize[1] / 2.0 + offset;

			center[0] = xTick[0] + x * sin(theta);
			center[1] = xTick[1] - y * cos(theta);

			auto offsetx = stringSize[0] / 2;/** sin(theta);*/
			auto offsety = stringSize[1] / 2;// *cos(theta);
		    pos[0] = static_cast<int>(center[0] + offsetx);
			pos[1] = static_cast<int>(center[1] + offsety+1);
			/*pos[0] = static_cast<int>(center[0] - stringSize[0] / 2.0);
			pos[1] = static_cast<int>(center[1] - stringSize[1] / 2.0);*/
			//TitleActor->SetPosition(center[0], center[1]);
			TitleActor->SetPosition(pos[0], pos[1]);

		}
		
		/*this->SetOffsetPosition(xTick,
			theta,
			stringSize[0],
			stringSize[1],
			static_cast<int>(offset),
			this->TitleActor);*/
	} // If title visible

	this->BuildTime.Modified();
}

void MyVtkAxisActor2D::SetLabelsNumber(int iNumber)
{
	NumberOfLabels = iNumber;
}
