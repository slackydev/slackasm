{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Utilities
[-----------------------------------------------------------------------------}

function GetIntSize(x: Int64; validSizes: TByteSet=[1,4]): Int32;
begin
  if (Low(Int8)  <= x) and (High(Int8)  >= x) and (1 in validSizes) then Exit(1);
  if (Low(Int16) <= x) and (High(Int16) >= x) and (2 in validSizes) then Exit(2);
  if (Low(Int32) <= x) and (High(Int32) >= x) and (4 in validSizes) then Exit(4);
  if (Low(Int64) <= x) and (High(Int64) >= x) and (8 in validSizes) then Exit(8);
end;

function GetUIntSize(x: UInt64; validSizes: TByteSet=[1,4]): Int32;
begin
  if (High(UInt8) >= x)  and (1 in validSizes) then Exit(1);
  if (High(UInt16) >= x) and (2 in validSizes) then Exit(2);
  if (High(UInt32) >= x) and (4 in validSizes) then Exit(4);
  if (High(UInt64) >= x) and (8 in validSizes) then Exit(4);
end;
