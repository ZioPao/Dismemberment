modded class ScriptedDamageManagerComponent : BaseScriptedDamageManagerComponent
{
	
	
	EDamageState lastState;


    //!	Invoked when damage state changes.
	protected override void OnDamageStateChanged(EDamageState state)
	{
		super.OnDamageStateChanged(state);
		
		
		
		if (m_iDamageManagerDataIndex == -1)
			return;
		
		
		lastState = state;

		
		
		ScriptInvoker invoker = s_aScriptedDamageManagerData[m_iDamageManagerDataIndex].GetOnDamageStateChanged(false);
		if (invoker)
			invoker.Invoke(state);
	}
	
}