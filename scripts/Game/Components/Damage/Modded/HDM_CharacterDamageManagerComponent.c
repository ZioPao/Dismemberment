modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{

	
	
	
	const ResourceName IDENTITY_HeadEyeDismembermentBlack = "{EE1E32B10A398E83}Prefabs/Identities/Black/IdentityHeadEyeDismemberment.et";
	const ResourceName IDENTITY_HeadFullDismembermentBlack = "{F1F57D8F72C5897E}Prefabs/Identities/Black/IdentityHeadFullDismemberment.et";
	const ResourceName IDENTITY_HeadMidDismembermentBlack = "{CA2D3383201D1745}Prefabs/Identities/Black/IdentityHeadMidDismemberment.et";
	
	
	const ResourceName IDENTITY_HeadEyeDismembermentWhite = "{45DAB89BBE306ADF}Prefabs/Identities/White/IdentityHeadEyeDismemberment.et";
	const ResourceName IDENTITY_HeadFullDismembermentWhite = "{76FA1045B6F21E20}Prefabs/Identities/White/IdentityHeadFullDismemberment.et";
	const ResourceName IDENTITY_HeadMidDismembermentWhite = "{50621BD94C7ABD0D}Prefabs/Identities/White/IdentityHeadMidDismemberment.et";
	
	const ResourceName PREFAB_genericWound = "{DA5DADA0ADB7CD05}Prefabs/Wounds/generic_amputation_wound.et";
	const ResourceName PREFAB_eyeWound = "{83BAC0D42B0D4CDE}Prefabs/Wounds/eye_wound.et";
	
	
	
	
	ref map<string, ref StructHeadDismemberment> ethnMap;
	
	
	
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
		
		
		if (pHitZone.GetName() == "Head" && lastState == EDamageState.DESTROYED)
			PerformHeadAmputation(damage);
		
		
		UpdateBloodyFace();
	}
	
	
	
	
	void DropHelmetOff()
	{
	
		//Storage container 
		BaseLoadoutManagerComponent baseLoadoutManagerComponent = BaseLoadoutManagerComponent.Cast(GetOwner().FindComponent(BaseLoadoutManagerComponent));
		
		
		IEntity helmetOnCharacter = baseLoadoutManagerComponent.GetClothByArea(ELoadoutArea.ELA_HeadCover);
		
		if (helmetOnCharacter)
		{
			ResourceName helmetResourceName = helmetOnCharacter.GetPrefabData().GetPrefabName();	

			baseLoadoutManagerComponent.Unwear(helmetOnCharacter);
			delete helmetOnCharacter;
			
			
			Resource helmetResource = Resource.Load(helmetResourceName);
			//helmetEntity.SetWorldTransform(GetOwner().GetOrigin());
			//helmetEntity.SetScale(1);
			//helmetEntity.GetPhysics().SetActive(1);
			
	
			EntitySpawnParams params = EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;
			GetOwner().GetWorldTransform(params.Transform);
			params.Transform[3][1] = params.Transform[3][1] + 1.7;
			IEntity helmetEntity = GetGame().SpawnEntityPrefab(helmetResource, null, params);
			
			helmetEntity.GetPhysics().Destroy();
			Physics.CreateDynamic(helmetEntity, 1.0, -1);
			helmetEntity.GetPhysics().SetActive(ActiveState.ACTIVE);
	
		}
	}
	

	void InitEthnMap()
	{
		ethnMap = new map<string, ref StructHeadDismemberment>;
		
		
		ethnMap.Insert("WhiteEye", new StructHeadDismemberment(IDENTITY_HeadEyeDismembermentWhite, PREFAB_eyeWound));
		ethnMap.Insert("WhiteMid", new StructHeadDismemberment(IDENTITY_HeadMidDismembermentWhite, PREFAB_genericWound));
		ethnMap.Insert("WhiteFull", new StructHeadDismemberment(IDENTITY_HeadFullDismembermentWhite, PREFAB_genericWound));

		
		ethnMap.Insert("BlackEye", new StructHeadDismemberment(IDENTITY_HeadEyeDismembermentBlack, PREFAB_eyeWound));
		ethnMap.Insert("BlackMid", new StructHeadDismemberment(IDENTITY_HeadMidDismembermentBlack, PREFAB_genericWound));
		ethnMap.Insert("BlackFull", new StructHeadDismemberment(IDENTITY_HeadFullDismembermentBlack, PREFAB_genericWound));
		
		
		
		
	}
	
	void PerformHeadAmputation(float damage)
	{
	
	/* CHANGE IDENTITY HERE */
		
		
		InitEthnMap();
		
		
		
		// Check head 
		CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast(GetOwner().FindComponent(CharacterIdentityComponent));
		ResourceName headResource = identityComponent.GetIdentity().GetHead();
		string key;
		
		if (headResource.Contains("Head_White"))
			key = "White";
		
		if (headResource.Contains("Head_Black"))
			key = "Black";
		else 
			key = "White";
		
		
		
		
		Resource identityResource;
		Resource woundResource;
		
		StructHeadDismemberment headStruct;

		int pivotId;
		float scale;
		vector rotation;
		

		// start dismeemberment set it to 11
		if (Math.RandomInt(0, 9) < 4)
		{
			
			// Minor Dismemberment
			if (damage < 80)
			{
				
				// Mid
				if (Math.RandomInt(0,2) == 0)
				{
					
					headStruct = ethnMap.Get(key + "Mid");
					pivotId = 2708386983;
					scale = 0.045;
					rotation = {0, 90, -30};		// WE NEED A DIFFERENT MODEL!!!
									//YAW NO 
					//PITCH !!! 
					//position = {}
				}
				// Eye dismemberment
				else
				{
					
					headStruct = ethnMap.Get(key + "Eye");
					pivotId = 256054695;
					scale = 0.027;
					rotation = {0, 20, 20};	
				
				}
			
			
			
			}
			// Full dismemberment
			else
			{
				headStruct = ethnMap.Get(key + "Full");
				pivotId = 268344569;
				scale = 0.04;
				rotation = {0,0,0};
			}		
			
								
		identityResource = Resource.Load(headStruct.headIdentity);
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

		else
			Print("Cant find identity");
		
		//it's not deleting them. No idea why. Can't even make them invisible ffs. 
		delete identityEntity;		//it's not working?
		
		
		
		
		//Spawn amputation 
		IEntity woundEntity = GetGame().SpawnEntityPrefab(woundResource);

		//armEntity.SetScale(0.04);
		
		
		woundEntity.SetScale(scale);
		woundEntity.SetYawPitchRoll(rotation);
		//vector woundOrigin = woundEntity.GetOrigin();
		
		//woundOrigin[1] = woundOrigin[1] + 0.02;
		//woundEntity.SetOrigin(woundOrigin);
		GetOwner().AddChild(woundEntity, pivotId);
		DropHelmetOff();
		}
		
		

		
		
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
		
		
		
		
		
		
		//GetCharacter().AddChild(rightArmEntity, 3241353630);
		
		
		
		//ResourceName identityName = "{FAE42E51C156837D}Configs/Identities/VisualIdentity_Dismemberment.conf";
		
		//Identity identityDismemberment = Identity.Cast(BaseContainerTools.LoadContainer(identityName));
		//identityComponent.SetIdentity(identity);
	
	}

}


class StructHeadDismemberment
{
	ResourceName headIdentity;
	ResourceName woundPrefab;
	
	
	void StructHeadDismemberment(ResourceName hi, ResourceName wp)
	{
		this.headIdentity = hi;
		this.woundPrefab = wp;
	}

}