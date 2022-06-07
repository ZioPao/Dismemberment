modded class ScriptedDamageManagerComponent : BaseScriptedDamageManagerComponent
{
	
	EDamageState lastState;

	protected override void OnDamageStateChanged(EDamageState state)
	{
		super.OnDamageStateChanged(state);
		lastState = state;

	}
	
}