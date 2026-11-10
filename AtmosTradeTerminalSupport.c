class checkItemCL
{
	bool lotState = false;
};


class Atmos_TradeTerminal_Support 
{	
	protected static ref Atmos_TradeTerminal_Support   m_TradeTerminalSupport;
	
	void Atmos_TradeTerminal_Support()
	{
		
	}
	
	void GetFromArchive(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	
	void SellFromGarage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void GetGarage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void TransferToBankMoney(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void GetBankData(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void SayNoSlots(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void SayToPlayer(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void KillRestrainedPlayer(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void BuyItem(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	void BuyCar(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	private bool checkItem(int idMarket)
	{
		
	}
	
	void DeleteSellItem(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		
	}
	
	private bool checkItem( EntityAI etnAI, PlayerIdentity sender )
	{
		
	}
	
	static Atmos_TradeTerminal_Support GetInstance()
    {
        if(!m_TradeTerminalSupport)
            m_TradeTerminalSupport = new Atmos_TradeTerminal_Support();

        return m_TradeTerminalSupport;
    }

    static void CleanInstance()
    {
        m_TradeTerminalSupport = null;
    }
};