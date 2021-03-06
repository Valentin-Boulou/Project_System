----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    17:38:00 04/22/2020 
-- Design Name: 
-- Module Name:    Processeur - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--use work.ALU;
--use work.Banc_de_Registre;
--use work.Memoire_des_Instructions;
--use work.Memoire_des_Donnees;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values


-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;


--https://stackoverflow.com/questions/49197291/connecting-components-in-vhdl-structural

entity Processeur is
	Port (CLK : in  STD_LOGIC;
	RST: in  STD_LOGIC);
end Processeur;

architecture Structural of Processeur is



signal IP: STD_LOGIC_VECTOR (7 downto 0);
signal LC_MemRE_B_to_W: STD_LOGIC; 
signal LC_DIEX_OP_to_Ctrl_Alu: STD_LOGIC_VECTOR (2 downto 0);
signal LC_EXMem_OP_to_RW: STD_LOGIC;
signal QA: STD_LOGIC_VECTOR (7 downto 0);
signal QB: STD_LOGIC_VECTOR (7 downto 0);
signal S: STD_LOGIC_VECTOR (7 downto 0);
signal sortie: STD_LOGIC_VECTOR (7 downto 0);
signal addr: STD_LOGIC_VECTOR (7 downto 0);

-- 0-7:OP | 8-15:A | 16-23:B | 24-31:C
signal LIDI: STD_LOGIC_VECTOR (31 downto 0);
signal DIEX: STD_LOGIC_VECTOR (31 downto 0);
signal EXMem: STD_LOGIC_VECTOR (31 downto 0);
signal MemRE: STD_LOGIC_VECTOR (31 downto 0);

-- TRASH TEMP
signal addrTEMP :  STD_LOGIC_VECTOR (3 downto 0);
signal QTEMP : STD_LOGIC_VECTOR (7 downto 0);

--Test
type i_table is array (0 to 3) of std_logic_vector(31 downto 0);
signal instr: i_table := (others=>(others=>'0'));


begin

	
	InstruMem : entity work.Memoire_des_Instructions port map ( IP, CLK, instr(0));
	BancReg : entity work.Banc_de_Registre port map (LIDI(19 downto 16),LIDI(27 downto 24),MemRE(11 downto 8),MemRE(23 downto 16),RST,CLK,QA,QB,LC_MemRE_B_to_W);
	UAL: entity work.ALU port map(DIEX(23 downto 16), DIEX(31 downto 24), LC_DIEX_OP_to_Ctrl_Alu,S);
	DataMem: entity work.Memoire_des_Donnees port map (addr,EXMem(23 downto 16), LC_EXMem_OP_to_RW, RST, CLK, sortie);
	
	
	process (CLK) is
	variable cpt: natural range 0 to 4 := 0;
	begin
		if(rising_edge(CLK)) then
			if(RST = '0') then
				IP <= "00000000";
			else
				if(cpt = 1) then 
					LIDI <= instr(0);
					IP <= IP + 1;
					cpt := cpt - 1 ;
				elsif( cpt = 2) then 
					LIDI <= (others => '0');
					IP <= IP + 1;
					cpt := cpt - 1 ;
				elsif( cpt > 2) then 
					LIDI <= (others => '0');
					IP <= IP;
					cpt := cpt - 1 ;
				elsif((((LIDI(15 downto 8) = instr(0)(23 downto 16) or LIDI(15 downto 8) = instr(0)(31 downto 24)) and instr(0)(7 downto 0) /= "00000000") or
						 ((LIDI(15 downto 8) = instr(1)(23 downto 16) or LIDI(15 downto 8) = instr(1)(31 downto 24)) and instr(1)(7 downto 0) /= "00000000") or
						 ((LIDI(15 downto 8) = instr(2)(23 downto 16) or LIDI(15 downto 8) = instr(2)(31 downto 24)) and instr(2)(7 downto 0) /= "00000000") or
						 ((LIDI(15 downto 8) = instr(3)(23 downto 16) or LIDI(15 downto 8) = instr(3)(31 downto 24)) and instr(3)(7 downto 0) /= "00000000") )) then
					LIDI <= (others => '0');
					IP <= IP - 1;
					cpt := 4;
				else
					LIDI <= instr(0);
					if(IP < "00101101") then
						IP <= IP + 1;
					end if;
				end if;
			end if;
			instr(1) <= instr(0);
			instr(2) <= instr(1);
			instr(3) <= instr(2);
			--A
			DIEX(15 downto 8) <= LIDI(15 downto 8);
			--OP
			DIEX(7 downto 0) <= LIDI(7 downto 0);
			--B
			if(LIDI(7 downto 0) = "00000101" or LIDI(7 downto 0) = "00000001" or LIDI(7 downto 0) = "00000010" or LIDI(7 downto 0) = "00000011") or LIDI(7 downto 0) = "00001000" then
				DIEX(23 downto 16) <=  QA;
			else
				DIEX(23 downto 16) <= LIDI(23 downto 16);
			end if;
			--C
			DIEX(31 downto 24) <= QB;
			
			--A
			EXMem(15 downto 8) <= DIEX(15 downto 8);
			--OP
			EXMem(7 downto 0) <= DIEX(7 downto 0);
			--B
			if(DIEX(7 downto 0) = "00000001" or DIEX(7 downto 0) = "00000010" or DIEX(7 downto 0) = "00000011") then
				EXMem(23 downto 16) <= S;
			else
				EXMem(23 downto 16) <= DIEX(23 downto 16);
			end if;
			--C
			EXMem(31 downto 24) <= (others=>'0');
			
			--A
			MemRE(15 downto 8) <= EXMem(15 downto 8);
			--OP
			MemRE(7 downto 0) <= EXMem(7 downto 0);
			--B
			if(EXMem(7 downto 0) = "00000111" or EXMem(7 downto 0) = "00001000") then
				MemRE(23 downto 16) <= sortie;
			else
				MemRE(23 downto 16) <= EXMem(23 downto 16);
			end if;
			--C
			MemRE(31 downto 24) <= (others=>'0');
			
			
		end if;
	end process;
	
		
	LC_MemRE_B_to_W <= '1' when MemRE(7 downto 0) = "00000110" or MemRE(7 downto 0) = "00000101" else
		'1' when MemRE(7 downto 0) = "00000001" or MemRE(7 downto 0) = "00000010" or MemRE(7 downto 0) = "00000011" else
		'1' when MemRE(7 downto 0) = "00000111" else
		'0';
	LC_DIEX_OP_to_Ctrl_Alu <=  "001" when DIEX(7 downto 0) = "00000011" else
		"010" when DIEX(7 downto 0) = "00000010" else
		"000" when DIEX(7 downto 0) = "00000001";
	LC_EXMem_OP_to_RW <= '0' when EXMem(7 downto 0) = "00001000" else
		'1';
	addr <= EXMem(15 downto 8) when EXMem(7 downto 0) = "00001000" else
		EXMem(23 downto 16);
	
--	--A
--	DIEX(15 downto 8) <= LIDI(15 downto 8);
--	--OP
--	DIEX(7 downto 0) <= LIDI(7 downto 0);
--	--B
--	DIEX(23 downto 16) <=  QA  when LIDI(7 downto 0) = "00000101" or LIDI(7 downto 0) = "00000001" or LIDI(7 downto 0) = "00000010" or LIDI(7 downto 0) = "00000011" else
--		QA when  LIDI(7 downto 0) = "00001000" else
--		LIDI(23 downto 16);
--	--C
--	DIEX(31 downto 24) <= QB;
	
--	--A
--	EXMem(15 downto 8) <= DIEX(15 downto 8);
--	--OP
--	EXMem(7 downto 0) <= DIEX(7 downto 0);
--	--B
--	EXMem(23 downto 16) <= S when DIEX(7 downto 0) = "00000001" or DIEX(7 downto 0) = "00000010" or DIEX(7 downto 0) = "00000011" else 
--		DIEX(23 downto 16);
	
--	--A
--	MemRE(15 downto 8) <= EXMem(15 downto 8);
--	--OP
--	MemRE(7 downto 0) <= EXMem(7 downto 0);
--	--B
--	MemRE(23 downto 16) <= sortie when EXMem(7 downto 0) = "00000111" or EXMem(7 downto 0) = "00001000" else
--		EXMem(23 downto 16);
	
	

end Structural;

