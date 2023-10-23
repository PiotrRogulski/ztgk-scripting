TextureCube envMap : register(t0);
Texture2D normTexture : register(t1);
sampler samp : register(s0);

float3 normalMapping(const float3 N, float3 T, const float3 tn) {
    const float3 B = normalize(cross(N, T));

    T = cross(B, N);

    const float3x3 m = {T, B, N};

    return mul(transpose(m), tn);
}

float3 intersectRay(const float3 position, const float3 direction) {
    /*
    position - P
    direction - R
    */

    float3 t_prim = max((1 - position) / direction, (-1 - position) / direction);

    return (position + min(t_prim.x, min(t_prim.y, t_prim.z)) * direction);
}

float fresnel(const float n1, const float n2, const float theta) {
    const float F0 = pow((n2 - n1) / (n2 + n1), 2);

    return F0 + (1 - F0) * pow(1 - theta, 5);
}

struct PSInput {
    float4 pos : SV_POSITION;
    float3 norm : NORMAL0;
    float3 localPos : POSITION0;
    float3 worldPos : POSITION1;
    float4 camPos : TEXCOORD0;
};

float4 main(PSInput i) : SV_TARGET {
    const float n1 = 1.0f;
    const float n2 = 4.0f / 3.0f;

    const float3 viewVec = normalize(i.camPos.xyz - i.worldPos);
    const float2 texCoord = (i.localPos.xz + float2(1.0f, 1.0f)) / 2.0f;

    /* Normal Mapping */
    const float3 N = normalize(i.norm);

    const float3 dPdx = ddx(i.worldPos);
    const float3 dPdy = ddy(i.worldPos);
    float2 dtdx = ddx(texCoord);
    float2 dtdy = ddy(texCoord);

    const float3 T = normalize(-dPdx * dtdy.y + dPdy * dtdx.y);

    float3 normal_from_texture = normTexture.Sample(samp, texCoord).xyz;
    normal_from_texture = 2 * normal_from_texture - float3(1.0f, 1.0f, 1.0f);

    float3 norm = normalize(normalMapping(N, T, normal_from_texture));

    /* Fressnel Reflection */
    const float3 reflect_vector = reflect(-viewVec, norm);
    float3 refract_vector;

    if (dot(norm, viewVec) > 0) /* Nad wodą */
    {
        refract_vector = refract(-viewVec, norm, n1 / n2);
    } else /* Pod wodą */
    {
        norm = -norm;
        refract_vector = refract(-viewVec, norm, n2 / n1);
    }

    const float3 tex_colour_reflect = envMap.Sample(samp, intersectRay(i.localPos, reflect_vector)).xyz;
    const float3 tex_colour_refract = envMap.Sample(samp, intersectRay(i.localPos, refract_vector)).xyz;

    float3 color;

    if (any(refract_vector)) /* Robienie ku�ka z efektem ca�kowitego wewn�trznego odbicia */
    {
        const float fresnel_value = fresnel(n1, n2, max(dot(norm, viewVec), 0.0f));
        color = lerp(tex_colour_refract, tex_colour_reflect, fresnel_value);
    } else {
        color = tex_colour_reflect;
    }

    //color += float3(0.0f, 0.1f, 0.2f);

    return float4(pow(color, 2), 1.0f);
}
