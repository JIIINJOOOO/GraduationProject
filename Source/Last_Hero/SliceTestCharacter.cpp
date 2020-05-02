// Fill out your copyright notice in the Description page of Project Settings.


#include "SliceTestCharacter.h"

// Sets default values
ASliceTestCharacter::ASliceTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASliceTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASliceTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASliceTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASliceTestCharacter::CopyMesh() 
{
	ProcMeshComponent->SetWorldTransform(GetMesh()->GetComponentTransform());

	if (ProcMeshComponent != nullptr) {
		int32 InOverallMaxLODs = 0;
		InOverallMaxLODs = FMath::Max(GetMesh()->MeshObject->GetSkeletalMeshResource().LODModels.Num(), InOverallMaxLODs);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "InOverallMaxLODs : " + FString::FromInt(InOverallMaxLODs));
		int32 LODIndex = 0;
		FSkeletalMeshLODInfo& SrcLODInfo = GetMesh()->SkeletalMesh->LODInfo[LODIndex];

		// Get the CPU skinned verts for this LOD
		TArray<FFinalSkinVertex> FinalVertices;
		GetMesh()->GetCPUSkinnedVertices(FinalVertices, LODIndex);

		FSkeletalMeshResource& SkeletalMeshResource = GetMesh()->MeshObject->GetSkeletalMeshResource();
		FStaticLODModel& StaticLODModel = SkeletalMeshResource.LODModels[LODIndex];

		uint32 Max_Coords = 8;

		const uint32 NumTexCoords = FMath::Min(StaticLODModel.VertexBufferGPUSkin.GetNumTexCoords(), Max_Coords);
		const int32 NumSections = StaticLODModel.Sections.Num();
		FRawStaticIndexBuffer16or32Interface& IndexBuffer = *StaticLODModel.MultiSizeIndexContainer.GetIndexBuffer();

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "NumSections : " + FString::FromInt(NumSections));

		//MyVertices as Vertices
		TArray<FVector> MyVertices;

		//WedgeIndices as Triangles?
		TArray<int32>MyTriangles;

		//WedgeTangetX as Tangent?
		TArray<FProcMeshTangent>MyTangent;

		//WedgeTangetZ as Normal?
		TArray<FVector>MyNormal;

		//WedgeTexCoords as UV?
		TArray<FVector2D> MyUV;

		TArray<FColor> Colors;

		// Map from vert buffer for whole mesh to vert buffer for section of interest
		TMap<int32, int32> MeshToSectionVertMap;

		FMatrix WorldToRoot = GetMesh()->GetComponentTransform().ToMatrixWithScale().Inverse();
		FMatrix ComponentToWorld = GetMesh()->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;
		//FMatrix ComponentToWorld = GetMesh()->GetComponentTransform().ToMatrixWithScale();
		for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++) {
			const FSkelMeshSection& SkelMeshSection = StaticLODModel.Sections[SectionIndex];
			if (!SkelMeshSection.bDisabled) {
				// Build 'wedge' info
				const int32 NumWedges = SkelMeshSection.NumTriangles * 3;
				for (int32 WedgeIndex = 0; WedgeIndex < NumWedges; WedgeIndex++) {

					const int32 VertexIndexForWedge = IndexBuffer.Get(SkelMeshSection.BaseIndex + WedgeIndex);

					int32* NewIndexPtr = MeshToSectionVertMap.Find(VertexIndexForWedge);
					if (NewIndexPtr != nullptr) {
						//Copy Triangles
						MyTriangles.Add(*NewIndexPtr);
					}
					else {
						// Copy position
						int32 SectionVertIndex = MyVertices.Add(FinalVertices[VertexIndexForWedge].Position);

						const FFinalSkinVertex& SkinnedVertex = FinalVertices[VertexIndexForWedge];
						const FVector TangentX = ComponentToWorld.TransformVector(SkinnedVertex.TangentX);
						FProcMeshTangent NewTangent(TangentX, 0);

						// Copy tangents
						MyTangent.Add(NewTangent);
						// Copy normal
						const FVector TangentZ = ComponentToWorld.TransformVector(SkinnedVertex.TangentZ);
						MyNormal.Add(TangentZ);
						// Copy UVs
						MyUV.Add(StaticLODModel.VertexBufferGPUSkin.GetVertexUV(VertexIndexForWedge, 0));

						MeshToSectionVertMap.Add(VertexIndexForWedge, SectionVertIndex);

						//Copy Triangles
						MyTriangles.Add(SectionVertIndex);

						if (StaticLODModel.ColorVertexBuffer.IsInitialized())
						{
							Colors.Add(StaticLODModel.ColorVertexBuffer.VertexColor(VertexIndexForWedge));
							//RawMeshTracker.bValidColors = true;
						}
						else
						{
							Colors.Add(FColor::White);
						}
					}
				}
			}
			TArray<FLinearColor> DummyColors;
			ProcMeshComponent->CreateMeshSection_LinearColor(SectionIndex, MyVertices, MyTriangles, MyNormal, MyUV, DummyColors, MyTangent, false);
		}

		// Clear any existing collision hulls
		ProcMeshComponent->ClearCollisionConvexMeshes();
		ProcMeshComponent->AddCollisionConvexMesh(MyVertices);

		//// MATERIALS
		for (int32 MatIndex = 0; MatIndex < GetMesh()->GetNumMaterials(); MatIndex++)
		{
			ProcMeshComponent->SetMaterial(MatIndex, GetMesh()->GetMaterial(MatIndex));
		}
		ProcMeshComponent->SetSimulatePhysics(true);
		ProcMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
