modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{

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
		
		
		
		// It's not health... But for now it'll work
		Print(damage);
		if (pHitZone.GetName() == "Head")
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
			helmetEntity.GetPhysics().SetActive(ActiveState.ALWAYS_ACTIVE);

			
		}
		
		
		

	
		
		//Print(what);
		//get reference
		
		
		//spawn on the ground
	
	}
	
	
	
	void PerformHeadAmputation(float damage)
	{
	
	/* CHANGE IDENTITY HERE */
		CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast(GetOwner().FindComponent(CharacterIdentityComponent));
				
		Resource resource;
		int pivotId;
		float scale;
		vector rotation;
		
		if (damage < 80)
		{
			
			
			
			
			int randomChance = Math.RandomInt(0,2);
			
			
			if (randomChance == 0)
			{
				resource = Resource.Load("{50621BD94C7ABD0D}Prefabs/Identities/IdentityHeadMidDismemberment.et");
				pivotId = 2708386983;
				scale = 0.045;
				rotation = {0, 90, -30};		// WE NEED A DIFFERENT MODEL!!!
								//YAW NO 
				//PITCH !!! 
				//position = {}

			}
			else
			{
				resource = Resource.Load("{45DAB89BBE306ADF}Prefabs/Identities/IdentityHeadEyeDismemberment.et");
				pivotId = 2708386983;
				scale = 0.045;
				rotation = {0, 90, -30};		
			}
			
		}
		else
		{
			resource = Resource.Load("{19F3924C8ABA9AB5}Prefabs/Identities/IdentityHeadFullDismemberment.et");
			pivotId = 268344569;
			scale = 0.04;
			rotation = {0,0,0};
		}
		
		
		
		
		IEntity testEntity = GetGame().SpawnEntityPrefab(resource);
		CharacterIdentityComponent tempIdentity = CharacterIdentityComponent.Cast(testEntity.FindComponent(CharacterIdentityComponent));
		
		if (tempIdentity)
			identityComponent.SetIdentity(tempIdentity.GetIdentity());

		else
			Print("Cant find identity");
		
		
		delete testEntity;
		
		
		
		
		//Spawn arm amputation 
		Resource woundResource = Resource.Load("{331292E4AF2D0499}Prefabs/wound_amputation.ent");			//Change name
		IEntity woundEntity = GetGame().SpawnEntityPrefab(woundResource);

		//armEntity.SetScale(0.04);
		
		
		woundEntity.SetScale(scale);
		woundEntity.SetYawPitchRoll(rotation);
		//vector woundOrigin = woundEntity.GetOrigin();
		
		//woundOrigin[1] = woundOrigin[1] + 0.02;
		//woundEntity.SetOrigin(woundOrigin);
		GetOwner().AddChild(woundEntity, pivotId);
		DropHelmetOff();
		

		
		
		//Right stuff needs to be arranged 
		
		//rightArmEntity.SetScale(0.04);
		//rightArmEntity.SetYawPitchRoll("0 0 -180");

				
		
		
		
		
		//LEFT ARM ID = 1885426032
		//LEFT ARM TWIST ID = 3566461163
		//RIGHT ARM TWIST ID = 3241353630
		
		
		
		// UPPER NECK ID = 268344569            ---------> Full dismemberment of head 
		// HEAD ID = 2090324343                 ---------> Mid dismbemberment of head
		// TONGUE ID = 2888468306				----------> Mid dismemberment of head BETTER
		
		
		
		
		
		
		//GetCharacter().AddChild(rightArmEntity, 3241353630);
		
		
		
		//ResourceName identityName = "{FAE42E51C156837D}Configs/Identities/VisualIdentity_Dismemberment.conf";
		
		//Identity identityDismemberment = Identity.Cast(BaseContainerTools.LoadContainer(identityName));
		//identityComponent.SetIdentity(identity);
	
	}

}