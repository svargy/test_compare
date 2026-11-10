class marketData
{
	int id;
	string itemname;
	string classname;
	string server_name;
	string player_name;
	string player_id;
	int price;
};

class restPlayerData
{
	int slots;
	string token;
	bool active;
	int balance;
	int srvId;
};

class AtmosTradeTerminalMenu : UIScriptedMenu
{
	
	string srvLink = "http://market.dayzatmosfera.ru/api/";
	ref restPlayerData m_restPlayerData = new restPlayerData;
	ref array< marketData > marketDataList_r = new array< marketData >;

	int srvId_market;
	int availableSlots;
	
	int bankAmmount;
	int terminalBalance;
			
	protected static ref AtmosTradeTerminalMenu Instance;
	bool isShow;
	bool isUnderstanding;
	ref array<EntityAI> itemsArray;
	PlayerBase m_Player;
	ref Atmos_TradeTerminal_Support atts;
	
	private TextListboxWidget 	m_listBox0;
	private TextWidget			m_label;
	private EditBoxWidget		m_edtSearch;
	private EditBoxWidget		m_edtSellPrice;
	private TextWidget			m_txtSellLabel;
	private TextWidget			m_txtBuyLabel;
	private TextWidget			m_txtPriceLabel;
	private RichTextWidget 		m_rtDisclaimer;
	private CheckBoxWidget		m_cbUnderstandItem;
	private CheckBoxWidget		m_cbOnlyMy;
	private Widget 				m_infoPanel;
	private ItemPreviewWidget 	m_ItemPreviewWidget;
	protected EntityAI 			previewItem;
	private MultilineTextWidget	m_txtItemLabel;
	
	private TextListboxWidget	m_listBoxItemForSell;
	
	private TextWidget			m_txtAvLots;
	private TextWidget			m_txtBalanceTrader;
	private TextWidget			m_txtBalanceBank;
	private TextWidget			m_txtAvLots_data;
	private TextWidget			m_txtBalanceTrader_data;
	private TextWidget			m_txtBalanceBank_data;
		
	private ButtonWidget		m_btnBuyItem;
	private ButtonWidget 		m_btnSellItem;
	private ButtonWidget 		m_btnRefresh;
	private ButtonWidget 		m_btnTransfer;
	private ButtonWidget		m_btnGetInfo;
	
	private ButtonWidget		m_btnTest2;
	
	
	protected ref Timer			m_CooldownTimer;
	protected ref Timer			m_DeleteFileTimer;
	
	void AtmosTradeTerminalMenu()
	{
		
		GetRPCManager().AddRPC("AtmosTradeTerminal", "RefreshSellItem", this, SingleplayerExecutionType.Server);
		GetRPCManager().AddRPC("AtmosTradeTerminal", "SetBankData", this, SingleplayerExecutionType.Server);
		itemsArray = new array<EntityAI>;
		
		m_CooldownTimer = new Timer();
		m_DeleteFileTimer = new Timer();
	}
	
	void ~AtmosTradeTerminalMenu()
	{
		GetGame().GetUIManager().ShowCursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(true);
	}
	
	void RefreshSellItem(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		GetPlayerInventory();
		GetRestPlayerData();
		GetRestData( true, m_cbOnlyMy.IsChecked());
	}
	
	void SetBankData(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1< int > data;
		if (!ctx.Read(data)) return;
		bankAmmount = data.param1;
		m_txtBalanceBank_data.SetText( bankAmmount.ToString());
	}
	
	static AtmosTradeTerminalMenu GetInstance()
	{
		if (!Instance)
        {
            Instance = new AtmosTradeTerminalMenu();
			Instance.Init();
        }
		return Instance;
	}
		
	static void ClearInstance()
	{
		Instance = null;
	}
	
	override Widget Init()
    {
		
		layoutRoot 				= GetGame().GetWorkspace().CreateWidgets("AtmosferaTradeTerminal/gui/tradeterminal.layout");
		m_listBox0 				= TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "listBox0" ) );
		m_listBoxItemForSell	= TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "listBoxItemForSell" ) );
		m_label					= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtLabel" ) );
		m_edtSearch				= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "edtSearch" ) );
		m_edtSellPrice			= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "edtSellPrice" ) );
		m_txtSellLabel			= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtSellLabel" ) );
		m_txtBuyLabel			= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtBuyLabel" ) );
		m_txtPriceLabel			= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtPriceLabel" ) );
		m_rtDisclaimer			= TextWidget.Cast( layoutRoot.FindAnyWidget( "rtDisclaimer" ) );
		m_cbUnderstandItem		= CheckBoxWidget.Cast( layoutRoot.FindAnyWidget( "cbUnderstandItem" ) );
		m_cbOnlyMy				= CheckBoxWidget.Cast( layoutRoot.FindAnyWidget( "cbOnlyMy" ) );

		m_txtAvLots				= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtAvLots" ) );
		m_txtBalanceTrader		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtBalanceTrader" ) );
		m_txtBalanceBank		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtBalanceBank" ) );
		m_txtAvLots_data		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtAvLots_data" ) );
		m_txtBalanceTrader_data	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtBalanceTrader_data" ) );
		m_txtBalanceBank_data	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtBalanceBank_data" ) );

		m_ItemPreviewWidget		= ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "ItemPreviewWidget0" ) );			
		m_txtItemLabel			= TextWidget.Cast( layoutRoot.FindAnyWidget( "txtItemLabel" ) );			
		
		m_btnBuyItem 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btnBuyItem" ) );
		m_btnSellItem			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btnSellItem" ) );
		m_btnRefresh 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btnRefresh" ) );
		m_btnTransfer 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btnTransfer" ) );
		m_btnGetInfo 			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btnGetInfo" ) );
		
		m_infoPanel				= layoutRoot.FindAnyWidget( "pnlInfo" );
		m_infoPanel.Show( false );
		
		m_btnTest2 		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btnTest2" ) );

		atts = new Atmos_TradeTerminal_Support();
		layoutRoot.Show(false);
		
        return layoutRoot;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		m_Player = g_Game.GetPlayer();
		
		isShow = true;
		isUnderstanding = false;
		
		GetPlayerInventory();
		
		m_label.SetText( "Атмосфера (торговый терминал)" );
		m_edtSearch.SetText( "поиск..." );
		m_txtSellLabel.SetText( "Продать" );
		m_txtBuyLabel.SetText( "Купить" );
		m_txtPriceLabel.SetText( "Цена:" );
		m_rtDisclaimer.SetText( "Если предмет содержит вложения, то они будут удалены!" );
		m_cbUnderstandItem.SetText( "Я понял" );
		m_edtSellPrice.SetText( "" );
				
		m_txtAvLots.SetText( "Доступно лотов:" );
		m_txtBalanceTrader.SetText( "Баланс (терминал):" );
		m_txtBalanceBank.SetText( "Баланс (банк):" );
		m_txtAvLots_data.SetText( "0" );
		m_txtBalanceTrader_data.SetText( "0" );
		m_txtBalanceBank_data.SetText( "0" );
		
		m_btnBuyItem.SetText( "Купить" );
		m_btnSellItem.SetText( "Продать" );
		m_btnTransfer.SetText( "Перевести" );
		m_btnRefresh.SetText( "Обновить" );
		m_btnGetInfo.SetText( "Инфо" );
		
		m_infoPanel.Show( false );
		
		m_cbOnlyMy.SetChecked( false );
		m_cbOnlyMy.SetText( "Только мои" );
	
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        GetGame().GetUIManager().ShowCursor( true );
		GetGame().GetMission().GetHud().Show( false );
        GetGame().GetInput().ChangeGameFocus(1);
		
		GetRestPlayerData();
		GetRestData();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		isShow = false;

        GetGame().GetUIManager().ShowCursor( false );
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable( true );
		GetGame().GetMission().GetHud().Show( true );
		ClearInstance();
		Close();
	}
	
	private void showInfoPanel()
	{
		string str_id;
		string itemClass;
		if( m_listBox0.GetSelectedRow() == -1 ) return;
		m_listBox0.GetItemText( m_listBox0.GetSelectedRow(), 0, str_id);
		for( int i = 0; i < marketDataList_r.Count(); i++ )
		{
			if( marketDataList_r[i].id == str_id.ToInt() )
			{
				itemClass = marketDataList_r[i].classname;
			}
		}
		
		if ( previewItem )
		{
			GetGame().ObjectDelete( previewItem );
			m_txtItemLabel.SetText( "" );
		}
		
		previewItem = GetGame().CreateObject( itemClass, "0 0 0", true, false, true );
		if( previewItem == null )
		{
			m_txtItemLabel.SetText( "Предмет не найден. Не рекомендуется к покупке" );
			m_infoPanel.Show( true );
			return;
		}
		InventoryItem iItem = InventoryItem.Cast( previewItem );
		
		m_txtItemLabel.SetText( iItem.GetTooltip() );
		
		m_ItemPreviewWidget.SetItem( previewItem );
		m_ItemPreviewWidget.SetModelPosition( Vector(0,0,0.5) );
		//InventoryItem.GetTooltip()
		m_infoPanel.Show( true );
	}

	override bool OnClick(Widget w, int x, int y, int button)
    {
		m_infoPanel.Show( false );
		if( previewItem ) GetGame().ObjectDelete( previewItem );
		
    	switch(w)
    	{
    		/*
			case m_btnTest1:
    			//m_listBoxItemForSell.RemoveRow( m_listBoxItemForSell.GetSelectedRow() );
				//GetPlayerInventory();
				return true;
    		break;
			*/
			
			case m_btnGetInfo:
    			//g_Game.GetUIManager().ShowDialog("Хорошо подумал?", "Вот прям точно хорошо подумал?", 67125, DBT_YESNO, DBB_YES, DMT_QUESTION, GetGame().GetUIManager().GetMenu());
				
				showInfoPanel();
				return true;
    		break;
			
			case m_btnRefresh:
				if( m_CooldownTimer.IsRunning() ) 
				{
					GetRPCManager().SendRPC("AtmosTradeTerminal", "SayToPlayer", new Param1< string >( "Не так быстро. Надо подождать еще " + m_CooldownTimer.GetRemaining().ToString().ToInt() + " сек." ), true ); 
					return false; 
				}
				m_CooldownTimer.Run(30, this, "ResetEvent", null);
				m_edtSearch.SetText( "" );	
				GetPlayerInventory();
				GetRestPlayerData();
				GetRestData( true );
				return true;
    		break;
			
			case m_cbUnderstandItem:
    			isUnderstanding = m_cbUnderstandItem.IsChecked();
				return true;
    		break;

			case m_cbOnlyMy:
				GetRestData( false, m_cbOnlyMy.IsChecked() );	
				return true;
    		break;

    		case m_btnBuyItem:
				if( m_CooldownTimer.IsRunning() ) {
					GetRPCManager().SendRPC("AtmosTradeTerminal", "SayToPlayer", new Param1< string >( "Не так быстро. Надо подождать еще " + m_CooldownTimer.GetRemaining().ToString().ToInt() + " сек." ), true ); 
					return false; 
				}
				m_CooldownTimer.Run(5, this, "ResetEvent", null);
				string str_id;
				m_listBox0.GetItemText( m_listBox0.GetSelectedRow(), 0, str_id);
				int buyRow = str_id.ToInt();
				if( buyRow <= 0 ) return false;
				string itemClass;
				int price;
				string str_price;
				m_listBox0.GetItemText( m_listBox0.GetSelectedRow(), 4, str_price);
				price = str_price.ToInt();
				for( int i = 0; i < marketDataList_r.Count(); i++ )
				{
					if( marketDataList_r[i].id == str_id.ToInt() )
				 	{
						itemClass = marketDataList_r[i].classname;
					}
				
					if( marketDataList_r[i].id == str_id.ToInt() && marketDataList_r[i].player_id == m_Player.m_Trader_PlayerUID )
				 	{
						price = 0;
					}
				}
				ItemBase m_ib = GetGame().CreateObject( itemClass, "0 0 0", true, false, false );
				if( m_ib == NULL )
				{
					GetRPCManager().SendRPC("AtmosTradeTerminal", "SayToPlayer", new Param1< string >( "Невозможно купить данный предмет. Вероятно, нет нужного мода на этом сервере" ), true ); 
					return false;
				}
				GetGame().ObjectDelete( m_ib );
				
				if( buyRow > 0 )
				{
	    			GetRPCManager().SendRPC("AtmosTradeTerminal", "BuyItem", new Param4< int , string, PlayerBase, int >( buyRow, itemClass, m_Player, price ), true ); 
				}
				return true;
    		break;
					
    		case m_btnSellItem:
				if( checkPlayer() )
				{
					int selRow = m_listBoxItemForSell.GetSelectedRow();
					//m_listBoxItemForSell.RemoveRow( selRow );
					if( m_cbUnderstandItem.IsChecked() && m_edtSellPrice.GetText().ToInt() > 0 && selRow > 0 )
					{
						EntityAI ent = itemsArray.Get( selRow + 1 );
						if( ent.GetType().Contains("ItemPhoto") )
						{
							GetRPCManager().SendRPC("AtmosTradeTerminal", "SayToPlayer", new Param1< string >( "Продажа данного предмета через терминал невозможна" ), true ); 
							return false;
						}
						GetRPCManager().SendRPC("AtmosTradeTerminal", "DeleteSellItem", new Param4< EntityAI, int, string, int >( ent, m_edtSellPrice.GetText().ToInt(), itemNameEncode(getItemDisplayName(ent.GetType())), srvId_market ), true ); 
					}
				}
				return true;
    		break;
			
			case m_btnTransfer:
				if( m_CooldownTimer.IsRunning() ) return false;
				m_CooldownTimer.Run(30, this, "ResetEvent", null);
    			GetRPCManager().SendRPC("AtmosTradeTerminal", "TransferToBankMoney", new Param3< PlayerBase, int, bool >( m_Player, terminalBalance, false ), true ); 
				return true;
    		break;
			
    	}
        return super.OnClick(w, x, y, button);
    }
	
	private bool checkPlayer()
	{
		if( m_CooldownTimer.IsRunning() ) {
			GetRPCManager().SendRPC("AtmosTradeTerminal", "SayToPlayer", new Param1< string >( "Не так быстро. Надо подождать еще " + m_CooldownTimer.GetRemaining().ToString().ToInt() + " сек." ), true ); 
			return false; 
		}
		m_CooldownTimer.Run(5, this, "ResetEvent", null);
		
		m_Player = g_Game.GetPlayer();
		if( m_Player.IsRestrained() )
		{
			GetRPCManager().SendRPC("AtmosTradeTerminal", "KillRestrainedPlayer", new Param1< PlayerBase >( m_Player ), true ); 
			return false;
		}
		
		if( availableSlots <= 0 )
		{
			GetRPCManager().SendRPC("AtmosTradeTerminal", "SayNoSlots"); 
			return false;
		}
		
		return true;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w,x,y,finished);
		
		switch(w)
		{
			case m_edtSellPrice:
				string onlyNumbers = ConstrainToPositiveNumber(m_edtSellPrice.GetText());
				//m_TextAmountInput.SetText(onlyNumbers);
        		m_edtSellPrice.SetText(onlyNumbers);
            	return true;
				break;
			case m_edtSearch:
				string filt = m_edtSearch.GetText();
				if( filt.Length() > 0 )
					filterListBox0( filt );
				if( filt.Length() == 0 )
					GetRestData( false );
            	return true;
				break;
        }
        return false;
	}
	
	protected void filterListBox0(string filt)
	{
		string i_name = "";
		m_listBox0.ClearItems();
		for( int i = 0; i < marketDataList_r.Count(); i++ )
		{ 
			i_name = marketDataList_r[i].itemname;
			i_name.ToLower();
			filt.ToLower();
			if( i_name.Contains( filt ) )
			{
				int dItemLB = m_listBox0.AddItem( marketDataList_r[i].id.ToString(), NULL, 0 );			
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].itemname, NULL, 1 );
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].server_name, NULL, 2 );
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].player_name, NULL, 3 );
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].price.ToString(), NULL, 4 );
				
				if( marketDataList_r[i].player_id == m_Player.m_Trader_PlayerUID )
				{
					m_listBox0.SetItemColor( dItemLB, 0, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 1, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 2, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 3, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 4, ARGBF(1, 0.4, 0.4, 1) );
				}
			}
		}
	}
	
	protected bool ContainsNumber(string input)
    {
		if(input.Contains("0"))
		{
			return true;
		}
		if(input.Contains("1"))
		{
			return true;
		}
		if(input.Contains("2"))
		{
			return true;
		}
		if(input.Contains("3"))
		{
			return true;
		}
		if(input.Contains("4"))
		{
			return true;
		}
		if(input.Contains("5"))
		{
			return true;
		}
		if(input.Contains("6"))
		{
			return true;
		}
		if(input.Contains("7"))
		{
			return true;
		}
		if(input.Contains("8"))
		{
			return true;
		}
		if(input.Contains("9"))
		{
			return true;
		}
		return false;
    }
	
	protected string ConstrainToPositiveNumber(string input)
    {
		string onlyNumbers = "";
		for(int i = 0; i<input.Length(); i++)
		{
			if(ContainsNumber(input.Get(i)))
			{
				onlyNumbers += input.Get(i);
			}
		}
		return onlyNumbers;
    }
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		//super.OnModalResult(w, x, y, code, result);
		
		if ( code == 67124 && result == DBB_YES )
		{
			
			return true;
			
		}
		else if ( code == 67124 && result == DBB_NO )
		{
			return false;
		}
		else if ( code == 67125 )
		{
			if ( result == DBB_YES )
			{
				
				return true;
				
			}
			else
			{
				return false;
			}
		}
		
		return false;
	}
	
	void GetPlayerInventory()
	{
		
		itemsArray.Clear();	
		m_listBoxItemForSell.SelectRow(-1);	
		m_listBoxItemForSell.ClearItems();
		m_Player = g_Game.GetPlayer();
		m_Player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

		ItemBase item;
		
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			Class.CastTo(item, itemsArray.Get(i));

			if( !item )
				continue;
					
			int strinv = m_listBoxItemForSell.AddItem( getItemDisplayName( item.GetType() ), null, 1 );
			m_listBoxItemForSell.SetItem( strinv, i.ToString(), null, 0 );
			
		}
		m_cbUnderstandItem.SetChecked( false );
		m_edtSellPrice.SetText( "" );
	}
		
	string getItemDisplayName(string itemClassname) // дубль
	{
		//TStringArray itemInfos = new TStringArray;
		
		string cfg = "CfgVehicles " + itemClassname + " displayName";
		string displayName;
		GetGame().ConfigGetText(cfg, displayName);
	
		if (displayName == "")
		{
			cfg = "CfgAmmo " + itemClassname + " displayName";
			GetGame().ConfigGetText(cfg, displayName);
		}
		
		if (displayName == "")
		{
			cfg = "CfgMagazines " + itemClassname + " displayName";
			GetGame().ConfigGetText(cfg, displayName);
		}
		
		if (displayName == "")
		{
			cfg = "cfgWeapons " + itemClassname + " displayName";
			GetGame().ConfigGetText(cfg, displayName);
		}
	
		if (displayName == "")
		{
			cfg = "CfgNonAIVehicles " + itemClassname + " displayName";
			GetGame().ConfigGetText(cfg, displayName);
		}
		
		
		if (displayName != "")
			return TrimUntPrefix(displayName);
		else
			return itemClassname;
	}
	
	string TrimUntPrefix(string str) // duplicate
	{
		str.Replace("$UNT$", "");
		return str;
	}
	
	void GetRestPlayerData()
	{
		if( FileExist("$profile:player_market.json") )
		{
			DeleteFile("$profile:player_market.json");
		}
		
		MenuData menu_data = g_Game.GetMenuData();
		int char_id = menu_data.GetLastPlayedCharacter();
		int port;
		string address,namesrv;
		menu_data.GetLastServerAddress(char_id,address);
		port = menu_data.GetLastServerPort(char_id);
		menu_data.GetLastServerName(char_id,namesrv);
		
		if (menu_data && menu_data.GetLastPlayedCharacter() != GameConstants.DEFAULT_CHARACTER_MENU_ID)
		{
		}
		else return;
		
		RestContext ctx = GetRestApi().GetRestContext(srvLink);
		ctx.SetHeader("application/json");

		//namesrv = plNameEncode( namesrv );
		string plName = m_Player.GetIdentity().GetName();
		plName = plNameEncode( plName );
		string data = ctx.GET_now( "player/" + m_Player.m_Trader_PlayerUID + "?name=" + plName + "&hostip=" + address + "&hostport=" + port.ToString() + "&hostname=" + namesrv );

		FileHandle file = OpenFile("$profile:player_market.json", FileMode.WRITE);
		FPrintln(file, data);
		CloseFile(file);
					
		JsonFileLoader< restPlayerData >.JsonLoadFile("$profile:player_market.json", m_restPlayerData);
		
		m_txtAvLots_data.SetText( m_restPlayerData.slots.ToString() );
		availableSlots = m_restPlayerData.slots;
		m_txtBalanceTrader_data.SetText( m_restPlayerData.balance.ToString() );
		terminalBalance = m_restPlayerData.balance;
		srvId_market = m_restPlayerData.srvId;
		
		if( FileExist("$profile:player_market.json") )
		{
			DeleteFile("$profile:player_market.json");
		}
		
		GetRPCManager().SendRPC("AtmosTradeTerminal", "GetBankData", new Param2< PlayerBase, bool >( m_Player, false ), true ); 
		
	}
	
	private string plNameEncode( string strIn )
	{
		string m_str = strIn;
		//m_str.Replace( " " , "%20" ); 
		//m_str.Replace( "!" , "%21" ); 
		//m_str.Replace( "*" , "%2A" ); 
		//m_str.Replace( "'" , "%27" ); 
		//m_str.Replace( "(" , "%28" ); 
		//m_str.Replace( ")" , "%29" ); 
		//m_str.Replace( ";" , "%3B" ); 
		//m_str.Replace( ":" , "%3A" ); 
		//m_str.Replace( "@" , "%40" ); 
		m_str.Replace( "&" , "%26" ); 
		//m_str.Replace( "=" , "%3D" ); 
		m_str.Replace( "+" , "%2B" ); 
		m_str.Replace( "$" , "%24" ); 
		m_str.Replace( "," , "%2C" ); 
		//m_str.Replace( "/" , "%2F" ); 
		//m_str.Replace( "?" , "%3F" ); 
		m_str.Replace( "%" , "%25" ); 
		m_str.Replace( "#" , "%23" ); 
		//m_str.Replace( "[" , "%5B" ); 
		//m_str.Replace( "]" , "%5D" );
		
		return m_str;
	}
	
	private string itemNameEncode( string strIn )
	{
		string m_str = strIn;
		//m_str.Replace( " " , "%20" ); 
		//m_str.Replace( "!" , "%21" ); 
		//m_str.Replace( "*" , "%2A" ); 
		m_str.Replace( "'" , " " ); 
		m_str.Replace( "«" , " " ); 
		m_str.Replace( "»" , " " ); 
		//m_str.Replace( "\" , " " ); 
		//m_str.Replace( "" , " " ); 
		//m_str.Replace( "(" , "%28" ); 
		//m_str.Replace( ")" , "%29" ); 
		//m_str.Replace( ";" , "%3B" ); 
		//m_str.Replace( ":" , "%3A" ); 
		//m_str.Replace( "@" , "%40" ); 
		m_str.Replace( "&" , " " ); 
		m_str.Replace( "=" , " " ); 
		m_str.Replace( "+" , " " ); 
		m_str.Replace( "$" , " " ); 
		m_str.Replace( "," , " " ); 
		m_str.Replace( "/" , " " ); 
		m_str.Replace( "\\" , " " ); 
		m_str.Replace( "?" , " " ); 
		m_str.Replace( "%" , " " ); 
		m_str.Replace( "#" , " " ); 
		m_str.Replace( "[" , " " ); 
		m_str.Replace( "]" , " " );
		
		return m_str;
	}
	
	void GetRestData( bool needLoad = true, bool onlyMy = false )
	{
		m_listBox0.ClearItems();
		
		if( needLoad )
		{
			if( FileExist("$profile:AtmosferaTradeTerminal/atmos_market.json") )
			{
				DeleteFile("$profile:AtmosferaTradeTerminal/atmos_market.json");
			}
			
			private MenuData menu_data = g_Game.GetMenuData();
			private int char_id = menu_data.GetLastPlayedCharacter();
			private string address,namesrv;
			menu_data.GetLastServerName(char_id,namesrv);
			//namesrv.Replace(" ","%20");
			
			RestCallback cbx1 = new RestCallback;
			RestContext ctx = GetRestApi().GetRestContext(srvLink);
			ctx.SetHeader("application/json");
			JsonSerializer js = new JsonSerializer;
			array< marketData > m_data = new array< marketData >;
			string errorstr;
			
			string data = ctx.GET_now( "market" + "?srvName=" + namesrv );
	
			FileHandle file = OpenFile("$profile:AtmosferaTradeTerminal/atmos_market.json", FileMode.WRITE);
			FPrintln(file, data);
			CloseFile(file);
			
			marketDataList_r.Clear();
			//marketDataList_r.Reserve();
			JsonFileLoader< array< marketData > >.JsonLoadFile("$profile:AtmosferaTradeTerminal/atmos_market.json", marketDataList_r);
			
			m_DeleteFileTimer.Run(10, this, "delTempFile", null);
			
			//if( FileExist("$profile:atmos_market.json") ) Print("ok");
		
		}
		
		for( int i = 0; i < marketDataList_r.Count(); i++ )
		{ 
			if( !onlyMy && i < 200 )
			{
				int dItemLB = m_listBox0.AddItem( marketDataList_r[i].id.ToString(), NULL, 0 );			
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].itemname, NULL, 1 );
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].server_name, NULL, 2 );
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].player_name, NULL, 3 );
				m_listBox0.SetItem( dItemLB, marketDataList_r[i].price.ToString(), NULL, 4 );
				
				if( bankAmmount > 0 && marketDataList_r[i].price > bankAmmount )
				{
					m_listBox0.SetItemColor( dItemLB, 4, ARGBF(1, 1, 0.4, 0.4) );
				}
				
				if( marketDataList_r[i].player_id == m_Player.m_Trader_PlayerUID )
				{
					m_listBox0.SetItemColor( dItemLB, 0, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 1, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 2, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 3, ARGBF(1, 0.4, 0.4, 1) );
					m_listBox0.SetItemColor( dItemLB, 4, ARGBF(1, 0.4, 0.4, 1) );
				}
			} 
			else 
			{
				if( marketDataList_r[i].player_id == m_Player.m_Trader_PlayerUID )
				{
					int dItemLBMy = m_listBox0.AddItem( marketDataList_r[i].id.ToString(), NULL, 0 );			
					m_listBox0.SetItem( dItemLBMy, marketDataList_r[i].itemname, NULL, 1 );
					m_listBox0.SetItem( dItemLBMy, marketDataList_r[i].server_name, NULL, 2 );
					m_listBox0.SetItem( dItemLBMy, marketDataList_r[i].player_name, NULL, 3 );
					m_listBox0.SetItem( dItemLBMy, marketDataList_r[i].price.ToString(), NULL, 4 );
					
					if( bankAmmount > 0 && marketDataList_r[i].price > bankAmmount )
					{
						m_listBox0.SetItemColor( dItemLBMy, 4, ARGBF(1, 1, 0.4, 0.4) );
					}
					
					if( marketDataList_r[i].player_id == m_Player.m_Trader_PlayerUID )
					{
						m_listBox0.SetItemColor( dItemLBMy, 0, ARGBF(1, 0.4, 0.4, 1) );
						m_listBox0.SetItemColor( dItemLBMy, 1, ARGBF(1, 0.4, 0.4, 1) );
						m_listBox0.SetItemColor( dItemLBMy, 2, ARGBF(1, 0.4, 0.4, 1) );
						m_listBox0.SetItemColor( dItemLBMy, 3, ARGBF(1, 0.4, 0.4, 1) );
						m_listBox0.SetItemColor( dItemLBMy, 4, ARGBF(1, 0.4, 0.4, 1) );
					}
				}
			}
			
			
		}
		
		if( FileExist("$profile:atmos_market.json") )
		{
			DeleteFile("$profile:atmos_market.json");
		}
	}
	
	protected void delTempFile()
	{
		m_DeleteFileTimer.Stop();
		if( FileExist("$profile:AtmosferaTradeTerminal/atmos_market.json") ) DeleteFile("$profile:AtmosferaTradeTerminal/atmos_market.json");
	}
	
	protected void ResetEvent()
	{
		m_CooldownTimer.Stop();
	}
};