struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};



PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    output.pos = input.pos;
    output.col = input.col;
	
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float4 col = input.col;
    int1 stripeHeight = 50;
    
    if (input.pos.y % stripeHeight > (stripeHeight / 2 - 1)) col = (0, 0, 0, 0);

    return col;
}