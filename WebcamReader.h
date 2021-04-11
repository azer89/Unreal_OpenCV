// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenCVHeaders.h"
#include "WebcamReader.generated.h"

// TODO
// This is based on
// https://nerivec.github.io/old-ue4-wiki/pages/detailed-account-of-integrating-opencv-into-ue4-with-vs2017.html
// https://github.com/adynathos/AugmentedUnreality/
// https://www.youtube.com/watch?v=xw6VMIStFxA


UCLASS()
class UNREAL_AR_API AWebcamReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWebcamReader();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// The device ID opened by the Video Stream
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		int32 CameraID;

	// If the webcam images should be resized every frame
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		bool ShouldResize;

	// The targeted resize width and height (width, height)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		FVector2D ResizeDimensions;

	// The rate at which the color data array and video texture is updated (in frames per second)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		float RefreshRate;

	// The refresh timer
	UPROPERTY(BlueprintReadWrite, Category = Webcam)
		float RefreshTimer;

	// If the stream has successfully opened yet
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		bool IsStreamOpen;

	// The videos width and height (width, height)
	UPROPERTY(BlueprintReadWrite, Category = Webcam)
		FVector2D VideoSize;

	// The current video frame's corresponding texture
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		UTexture2D* VideoTexture;

	// The current data array
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		TArray<FColor> Data;

public:
	// Blueprint Event called every time the video frame is updated
	UFUNCTION(BlueprintImplementableEvent, Category = Webcam)
		void OnNextVideoFrame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// OpenCV fields
	cv::Mat CVFrame;
	cv::VideoCapture CVVideo;
	cv::Size CVSize;

	// Pointer to update texture region 2D struct
	FUpdateTextureRegion2D* VideoUpdateTextureRegion;

private:
	// OpenCV prototypes
	void UpdateFrame();
	void UpdateTexture();

	// Use this function to update the texture rects you want to change:
	// NOTE: There is a method called UpdateTextureRegions in UTexture2D but it is compiled WITH_EDITOR
	// and is not marked as ENGINE_API so it cannot be linked from plugins.
	// FROM: Dynamic Textures
	void UpdateTextureRegions(UTexture2D* Texture, 
							  int32 MipIndex, 
						      uint32 NumRegions, 
							  FUpdateTextureRegion2D* Regions, 
							  uint32 SrcPitch, 
							  uint32 SrcBpp, 
							  uint8* SrcData, 
							  bool bFreeData);
};
