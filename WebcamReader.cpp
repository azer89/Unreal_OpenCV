// Fill out your copyright notice in the Description page of Project Settings.

#include "WebcamReader.h"
#include "Rendering/Texture2DResource.h"

// Sets default values
AWebcamReader::AWebcamReader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialize OpenCV and webcam properties
	CameraID = 0;
	RefreshRate = 15;
	IsStreamOpen = false;
	VideoSize = FVector2D(0, 0);
	ShouldResize = false;
	ResizeDimensions = FVector2D(320, 240);
	RefreshTimer = 0.0f;
	CVVideo = cv::VideoCapture();
	CVFrame = cv::Mat();
}

// Called when the game starts or when spawned
void AWebcamReader::BeginPlay()
{
	Super::BeginPlay();

	// Open the stream
	CVVideo.open(CameraID);
	if (CVVideo.isOpened())
	{
		// Initialize stream
		IsStreamOpen = true;
		UpdateFrame();
		VideoSize = FVector2D(CVFrame.cols, CVFrame.rows);
		CVSize = cv::Size(ResizeDimensions.X, ResizeDimensions.Y);
		VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
		VideoTexture->UpdateResource();
		VideoUpdateTextureRegion = 
			new FUpdateTextureRegion2D(0, 
				0, 
				0, 
				0, 
				VideoSize.X, 
				VideoSize.Y);

		// Initialize data array
		Data.Init(FColor(0, 0, 0, 255), 
				  VideoSize.X * VideoSize.Y);

		// Do first frame
		UpdateTexture();
		OnNextVideoFrame();
	}
	
}

// Called every frame
void AWebcamReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RefreshTimer += DeltaTime;
	if (IsStreamOpen && RefreshTimer >= 1.0f / RefreshRate)
	{
		RefreshTimer -= 1.0f / RefreshRate;
		
		UpdateFrame();
		UpdateTexture();
		OnNextVideoFrame();
	}
}

void AWebcamReader::UpdateFrame()
{
	if (CVVideo.isOpened())
	{
		CVVideo.read(CVFrame);
		if (ShouldResize)
		{
			cv::resize(CVFrame, CVFrame, CVSize);
		}
	}
	else 
	{
		IsStreamOpen = false;
	}
}

void AWebcamReader::UpdateTexture()
{
	if (IsStreamOpen && CVFrame.data)
	{
		// Copy Mat data to Data array
		for (int y = 0; y < VideoSize.Y; y++)
		{
			for (int x = 0; x < VideoSize.X; x++)
			{
				int i = x + (y * VideoSize.X);
				Data[i].B = CVFrame.data[i * 3 + 0];
				Data[i].G = CVFrame.data[i * 3 + 1];
				Data[i].R = CVFrame.data[i * 3 + 2];
			}
		}

		UpdateTextureRegions(VideoTexture, 
			(int32)0, 
			(uint32)1, 
			VideoUpdateTextureRegion, 
			(uint32)(4 * VideoSize.X), 
			(uint32)4, 
			(uint8*)Data.GetData(), 
			false);
	}
}



/*
	Special thanks to
	https://agin0634.github.io/web/ue4-opencv-opencv-webcam.html
*/
void AWebcamReader::UpdateTextureRegions(
	UTexture2D* Texture, 
	int32 MipIndex, 
	uint32 NumRegions, 
	FUpdateTextureRegion2D* Regions, 
	uint32 SrcPitch, 
	uint32 SrcBpp, 
	uint8* SrcData, 
	bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)
		(
			[RegionData, bFreeData](FRHICommandListImmediate& RHICmdList)
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
				if (bFreeData)
				{
					FMemory::Free(RegionData->Regions);
					FMemory::Free(RegionData->SrcData);
				}
				delete RegionData;
			}
		);
	}
}