modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{

	
	
	
	const ResourceName IDENTITY_HeadEyeDismembermentBlack = "{EE1E32B10A398E83}Prefabs/Identities/Black/IdentityHeadEyeDismemberment.et";
	const ResourceName IDENTITY_HeadFullDismembermentBlack = "{F1F57D8F72C5897E}Prefabs/Identities/Black/IdentityHeadFullDismemberment.et";
	const ResourceName IDENTITY_HeadMidDismembermentBlack = "{CA2D3383201D1745}Prefabs/Identities/Black/IdentityHeadMidDismemberment.et";
	const ResourceName IDENTITY_HeadThroughDismembermentBlack = "{16B4CED2E5822753}Prefabs/Characters/Identities/Black/IdentityHeadThroughDismemberment.et";		//temp

	
	const ResourceName IDENTITY_HeadEyeDismembermentWhite = "{45DAB89BBE306ADF}Prefabs/Identities/White/IdentityHeadEyeDismemberment.et";
	const ResourceName IDENTITY_HeadFullDismembermentWhite = "{76FA1045B6F21E20}Prefabs/Identities/White/IdentityHeadFullDismemberment.et";
	const ResourceName IDENTITY_HeadMidDismembermentWhite = "{50621BD94C7ABD0D}Prefabs/Identities/White/IdentityHeadMidDismemberment.et";
	const ResourceName IDENTITY_HeadThroughDismembermentWhite = "{2161B15B0163AC1B}Prefabs/Characters/Identities/White/IdentityHeadThroughDismemberment.et";
	
	
	const ResourceName IDENTITY_LegsFullDismembermentWhite = "{E2770B0493ACCD1E}Prefabs/Characters/Identities/White/IdentityBodyLegsDismemberment.et";
	const ResourceName IDENTITY_LegsFullDismembermentBlack = "{DC9291F282360398}Prefabs/Characters/Identities/Black/IdentityBodyLegsDismemberment.et";
	
	
	
	
	
	
	const ResourceName PREFAB_genericWound = "{DA5DADA0ADB7CD05}Prefabs/Wounds/generic_amputation_wound.et";
	const ResourceName PREFAB_eyeWound = "{83BAC0D42B0D4CDE}Prefabs/Wounds/eye_wound.et";
	const ResourceName PREFAB_legsWound = "{9445A37B351074F5}Prefabs/Wounds/legs_wound.et";

	
	
	
	ref map<string, ref StructIdentityDismemberment> ethnMap;
	
	
	float probabilityHeadDismemberment;
	float probabilityLegsDismemberment;

	
	ref map<string, string> settings;
	

	

	
	protected override void OnDamage(
				EDamageType type,
				float damage,
				HitZone pHitZone,
				IEntity instigator, 
				inout vector hitTransform[3], 
				float speed,
				int colliderID, 
				int nodeID)
	{
		super.OnDamage(type, damage, pHitZone, instigator, hitTransform, speed, colliderID, nodeID);
		
		Print(pHitZone.GetName());
		
		if (lastState == EDamageState.DESTROYED && damage > 75)
		{
			
			
			// should be done ONLY ONE TIME

			MCF_SettingsManager mcfSettingsManager = MCF_SettingsManager.GetInstance();
			
			const string fileNameJson = "DMD_Settings.json";
			const string MOD_ID = "5986A0819747D0B5";				
			map<string, string> mapDefaultValues = new map<string, string>();
				
				
			mapDefaultValues.Set("probabilityHeadDismemberment", "50");
			mapDefaultValues.Set("probabilityLegsDismemberment", "50");
	
				
				
			array<string> userFriendlyNames = {"Head Dismemberment Probability", "Legs Dismemberment Probability"};
			settings = mcfSettingsManager.Setup(MOD_ID, fileNameJson, mapDefaultValues, userFriendlyNames);
			probabilityHeadDismemberment = settings.Get("probabilityHeadDismemberment").ToInt();
			probabilityLegsDismemberment = settings.Get("probabilityLegsDismemberment").ToInt();
				
			Print("Initialized stuff for DMD");
			
			// SETTINGS STUFF
			probabilityHeadDismemberment = settings.Get("probabilityHeadDismemberment").ToInt();
			probabilityLegsDismemberment = settings.Get("probabilityLegsDismemberment").ToInt();
			
			
			InitEthnMap();
			
			
			int randomProbability;
			
			switch(pHitZone.GetName())
			{
				case "Head":
				{
					randomProbability = Math.RandomInt(0,100);
					Print(randomProbability);
					if (randomProbability <= probabilityHeadDismemberment)
						PerformHeadAmputation(damage);
					break;
				}
				case "LThigh":
				case "RThigh":
				case "LCalf":
				case "RCalf":
				
				{
					randomProbability = Math.RandomInt(0,100);
					Print(randomProbability);
					if (damage > 100 && randomProbability <= probabilityLegsDismemberment)
						PerformLegAmputation();
					break;
				}
			
			}
		
		}


			//PerformHeadAmputation(damage);
		
		
		
		
		//map<string,string> paramsMaterial = new map<string,string>();
		
		//ResourceName bcrMap = "
		//paramsMaterial.Insert("BCRMap", PBRCubeMap);
		//Material.Create("testMaterial", "MatPBRBasic", 
		
		
		UpdateBloodyFace();
	}
	
	


	void InitEthnMap()
	{
		ethnMap = new map<string, ref StructIdentityDismemberment>;
		
		
		
		// todo add for legs 
		
		
		
		ethnMap.Insert("WhiteHeadEye", new StructIdentityDismemberment(IDENTITY_HeadEyeDismembermentWhite, PREFAB_eyeWound));
		ethnMap.Insert("WhiteHeadMid", new StructIdentityDismemberment(IDENTITY_HeadMidDismembermentWhite, PREFAB_genericWound));
		ethnMap.Insert("WhiteHeadFull", new StructIdentityDismemberment(IDENTITY_HeadFullDismembermentWhite, PREFAB_genericWound));
		ethnMap.Insert("WhiteHeadThrough", new StructIdentityDismemberment(IDENTITY_HeadThroughDismembermentWhite, PREFAB_eyeWound));

		
		ethnMap.Insert("BlackHeadEye", new StructIdentityDismemberment(IDENTITY_HeadEyeDismembermentBlack, PREFAB_eyeWound));
		ethnMap.Insert("BlackHeadMid", new StructIdentityDismemberment(IDENTITY_HeadMidDismembermentBlack, PREFAB_genericWound));
		ethnMap.Insert("BlackHeadFull", new StructIdentityDismemberment(IDENTITY_HeadFullDismembermentBlack, PREFAB_genericWound));
		ethnMap.Insert("BlackHeadThrough", new StructIdentityDismemberment(IDENTITY_HeadThroughDismembermentBlack, PREFAB_eyeWound));

		
		ethnMap.Insert("WhiteLegsFull", new StructIdentityDismemberment(IDENTITY_LegsFullDismembermentWhite, PREFAB_legsWound));
		
		ethnMap.Insert("BlackLegsFull", new StructIdentityDismemberment(IDENTITY_LegsFullDismembermentBlack, PREFAB_legsWound));

		
		
		
		
	}
	
	
	
	string GetCorrectKey(CharacterIdentityComponent identityComponent, EDismembermentType type)
	{
		
		
		ResourceName bodyPartResourceName;
		
		switch(type)
		{
			case EDismembermentType.HEAD:
			{
				bodyPartResourceName = identityComponent.GetIdentity().GetHead();
				break;
			}
			case EDismembermentType.LEGS:
			{
				bodyPartResourceName = identityComponent.GetIdentity().GetBody();
				break;
			}
		}
		
		
		string key;
		
		if (bodyPartResourceName.Contains("White"))
			key = "White";
		
		if (bodyPartResourceName.Contains("Black") || bodyPartResourceName.Contains("African"))
			key = "Black";
		else 
			key = "White";
		
		return key;
	
	}
	
	
	void RemoveCloth(ELoadoutArea loadoutType, bool destroyCloth = false, float yMod = 0)
	{
	
		//Storage container 
		BaseLoadoutManagerComponent baseLoadoutManagerComponent = BaseLoadoutManagerComponent.Cast(GetOwner().FindComponent(BaseLoadoutManagerComponent));
		
		IEntity cloth = baseLoadoutManagerComponent.GetClothByArea(loadoutType);
		
		if (cloth)
		{
			ResourceName clothResourceName = cloth.GetPrefabData().GetPrefabName();	
			baseLoadoutManagerComponent.Unwear(cloth);
			
			delete cloth;
			
			if (!destroyCloth)
			{
				Resource clothResource = Resource.Load(clothResourceName);
	
				EntitySpawnParams params = EntitySpawnParams();
				params.TransformMode = ETransformMode.WORLD;
				GetOwner().GetWorldTransform(params.Transform);
				params.Transform[3][1] = params.Transform[3][1] + yMod;
				IEntity clothEntity = GetGame().SpawnEntityPrefab(clothResource, null, params);
				
				clothEntity.GetPhysics().Destroy();
				Physics.CreateDynamic(clothEntity, 1.0, -1);
				clothEntity.GetPhysics().SetActive(ActiveState.ACTIVE);
			}

		}
		
	
	
	}
	

	
	void PerformLegAmputation()
	{

		CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast(GetOwner().FindComponent(CharacterIdentityComponent));
		string key = GetCorrectKey(identityComponent, EDismembermentType.LEGS);
		StructIdentityDismemberment legsStruct = ethnMap.Get(key + "LegsFull");

		int pivotIdLeft = 78904579;
		int pivotIdRight = 4048764342;
			
		float scale = 0.06;
			
		Resource identityResource = Resource.Load(legsStruct.identity);
		Resource woundResource = Resource.Load(legsStruct.woundPrefab);		
						
				
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetOwner().GetWorldTransform(params.Transform);
		params.Transform[3][1] = params.Transform[3][1] - 10000;
				
			
			
		Print("Spawning identity and getting component");
		IEntity identityEntity = GetGame().SpawnEntityPrefab(identityResource, null, params);
		CharacterIdentityComponent tempIdentity = CharacterIdentityComponent.Cast(identityEntity.FindComponent(CharacterIdentityComponent));
			
			
			
			
		if (tempIdentity)
		{
			identityComponent.SetIdentity(tempIdentity.GetIdentity());
			identityComponent.Deactivate(identityEntity);
		}
	
			
			
		//it's not deleting them. No idea why. Can't even make them invisible ffs. 
		delete identityEntity;		//it's not working?
			
			
			
			
		
		//Spawn amputation 
		IEntity woundEntityLeft = GetGame().SpawnEntityPrefab(woundResource);
		IEntity woundEntityRight = GetGame().SpawnEntityPrefab(woundResource);
	
		woundEntityLeft.SetScale(scale);
		woundEntityRight.SetScale(scale);
	
		GetOwner().AddChild(woundEntityLeft, pivotIdLeft);
		GetOwner().AddChild(woundEntityRight, pivotIdRight);
	
		vector rotation = {0, 0, 180};
		woundEntityRight.SetYawPitchRoll(rotation);
			
		RemoveCloth(ELoadoutArea.ELA_Pants, true, 0);
		RemoveCloth(ELoadoutArea.ELA_Boots, true, 0);

		
		
		
		
	
			
	}
	
	
	void PerformHeadAmputation(float damage)
	{
	
	/* CHANGE IDENTITY HERE */

		
		// Check head 
		CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast(GetOwner().FindComponent(CharacterIdentityComponent));

		string key = GetCorrectKey(identityComponent, EDismembermentType.HEAD);
		
		
		Resource identityResource;
		Resource woundResource;
		
		StructIdentityDismemberment headStruct;

		int pivotId;
		float scale;
		vector rotation;	
		float diffX = 0;
		float diffY = 0;
		float diffZ = 0;
		bool doubleWound = false;
		
		
		
		
		
		
		StructParamsMeshes meshesParams;
		
		
		
		

		// start dismeemberment set it to (0,9)		
		// this is so bad but yeeeah.

		// Minor Dismemberment
		if (damage < 80)
		{
			int randomChance = Math.RandomInt(0,11);
			switch(randomChance)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				{
					headStruct = ethnMap.Get(key + "HeadMid");
					meshesParams = StructParamsMeshes("0 0 0", "0 90 -30", 0.045, 2708386983);
					break;		
				}	
				case 4:
				case 5:
				case 6:
				{				
					headStruct = ethnMap.Get(key + "HeadEye");
					meshesParams = StructParamsMeshes("0 0 0", "90 -30 -20", 0.06, 256054695);
					break;
				}		
				case 7:
				case 8:
				case 9:
				case 10:
				{			
					headStruct = ethnMap.Get(key + "HeadThrough");			
					meshesParams = StructParamsMeshes("0.013 0.012 0.02", "0 20 0", 0.04, 1850873948);
					doubleWound = true;
					break;
					
					// todo we should spawn TWO meshes for this case.
				}
			}	
				
				
		}	
			
		// Full dismemberment
		else
		{
			headStruct = ethnMap.Get(key + "HeadFull");
			meshesParams = StructParamsMeshes("0 0 0", "0 0 0", 0.04, 268344569);
		}		
			
								
		identityResource = Resource.Load(headStruct.identity);
		woundResource = Resource.Load(headStruct.woundPrefab);		
					
			
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		GetOwner().GetWorldTransform(params.Transform);
		params.Transform[3][1] = params.Transform[3][1] - 10000;
			
		
		IEntity identityEntity = GetGame().SpawnEntityPrefab(identityResource, null, params);
		CharacterIdentityComponent tempIdentity = CharacterIdentityComponent.Cast(identityEntity.FindComponent(CharacterIdentityComponent));
		
		if (tempIdentity)
		{
			identityComponent.SetIdentity(tempIdentity.GetIdentity());
			identityComponent.Deactivate(identityEntity);
			
			
		}

		
		//it's not deleting them. No idea why. Can't even make them invisible ffs. 
		delete identityEntity;		//it's not working?
		
		
		
		
		//Spawn amputation 
		//params = EntitySpawnParams();
		//params.TransformMode = ETransformMode.LOCAL;
		//GetOwner().GetLocalTransform(params.Transform);
		
		//params.Transform[3][0] = params.Transform[3][0] + 0.000005 ;
			

		//armEntity.SetScale(0.04);
		
		
		
		//IEntity woundEntity = GetGame().SpawnEntityPrefab(woundResource);
		//woundEntity.SetScale(scale);
		//woundEntity.SetYawPitchRoll(rotation);
		
		//vector woundOrigin = woundEntity.GetOrigin();
		//woundOrigin[0] = woundOrigin[0] - diffX;		// shift it a little to the left
		//woundOrigin[1] = woundOrigin[1] - diffY;		//move them back in the skull
		//woundOrigin[2] = woundOrigin[2] - diffZ;		// a little higher
		//woundEntity.SetOrigin(woundOrigin);
		

		//GetOwner().AddChild(woundEntity, pivotId);
		
		
		
		SpawnAndApplyWoundMesh(woundResource, meshesParams);

		if (doubleWound)
		{
			
			meshesParams.rotation[1] = -10;
			meshesParams.rotation[2] = meshesParams.rotation[2] + 180;	
			meshesParams.diffY = meshesParams.diffY + 0.176;
			SpawnAndApplyWoundMesh(woundResource, meshesParams);

			/*
			IEntity secondWoundEntity = GetGame().SpawnEntityPrefab(woundResource);
			secondWoundEntity.SetScale(scale);
			
			
			rotation[1] = -10;
			rotation[2] = rotation[2] + 180;

			
			secondWoundEntity.SetYawPitchRoll(rotation);		//todo should apply rotation
		
			vector secondWoundOrigin = secondWoundEntity.GetOrigin();
			secondWoundOrigin[0] = secondWoundOrigin[0] - diffX;		// shift it a little to the left
			secondWoundOrigin[1] = secondWoundOrigin[1] - diffY - 0.176;		//move them back in the skull
			secondWoundOrigin[2] = secondWoundOrigin[2] - diffZ;		// a little higher
			secondWoundEntity.SetOrigin(secondWoundOrigin);
		

			GetOwner().AddChild(secondWoundEntity, pivotId);
			*/
		
		}
		
		
		
		
		
		RemoveCloth(ELoadoutArea.ELA_HeadCover, false, 1.7);
		
		
		

		
		
		//Right stuff needs to be arranged 
		
		//rightArmEntity.SetScale(0.04);
		//rightArmEntity.SetYawPitchRoll("0 0 -180");

				
		
		
		
		
		//LEFT ARM ID = 1885426032
		//LEFT ARM TWIST ID = 3566461163
		//RIGHT ARM TWIST ID = 3241353630
		
		
		
		// UPPER NECK ID = 268344569            ---------> Full dismemberment of head 
		// HEAD ID = 2090324343                 ---------> Mid dismbemberment of head
		// TONGUE ID = 2888468306				----------> Mid dismemberment of head BETTER
		// RIGHT EYE ID = 256054695
		
		// RIGHT BROW ID = 1850873948			
		
		
		
		
		//GetCharacter().AddChild(rightArmEntity, 3241353630);
		
		
		
		//ResourceName identityName = "{FAE42E51C156837D}Configs/Identities/VisualIdentity_Dismemberment.conf";
		
		//Identity identityDismemberment = Identity.Cast(BaseContainerTools.LoadContainer(identityName));
		//identityComponent.SetIdentity(identity);
	
	}
	
	
	
	
	void SpawnAndApplyWoundMesh(Resource woundRes, StructParamsMeshes params)
	{
		
		IEntity woundEntity = GetGame().SpawnEntityPrefab(woundRes);
		woundEntity.SetScale(params.scale);
		woundEntity.SetYawPitchRoll(params.rotation);
		
		vector woundOrigin = woundEntity.GetOrigin();
		woundOrigin[0] = woundOrigin[0] - params.diffX;		// shift it a little to the left
		woundOrigin[1] = woundOrigin[1] - params.diffY;		//move them back in the skull
		woundOrigin[2] = woundOrigin[2] - params.diffZ;		// a little higher
		woundEntity.SetOrigin(woundOrigin);
		

		GetOwner().AddChild(woundEntity, params.pivotId);
		
		
		
	}
	
	
	
	

}


enum EDismembermentType
{
	HEAD,
	LEGS
}







class StructIdentityDismemberment
{
	ResourceName identity;
	ResourceName woundPrefab;
	
	
	void StructIdentityDismemberment(ResourceName ident, ResourceName wp)
	{
		this.identity = ident;
		this.woundPrefab = wp;
	}

}


class StructParamsMeshes
{
	float diffX;
	float diffY;
	float diffZ;
	
	float scale;
	vector rotation;
	
	int pivotId;
	
	
	void StructParamsMeshes(vector diff, vector rot, float sc, int pivot)
	{
		this.diffX = diff[0];
		this.diffY = diff[1];
		this.diffZ = diff[2];
		
		this.scale = sc;
		this.rotation = rot;
		this.pivotId = pivot;
	
	}

}