#ifndef USER_INTERFACE_H_INCLUDED
#define USER_INTERFACE_H_INCLUDED

#include <windows.h>

//Top menu
#define ID_TOP_MENU 10001
#define ID_SAVE_ART 10002
#define ID_LOAD_ART 10003
#define ID_SHOW_GRIDLINES 10004

//Buttons

//Draw area squares
//IMPORTANT: DRAW_AREA_WIDTH is the number of squares in a single row
#define DRAW_AREA_WIDTH 16
//IMPORTANT: DRAW_AREA_HEIGHT is the number of squares in a single column
#define DRAW_AREA_HEIGHT 16
#define DRAW_AREA_TOP_LEFT_X 10
#define DRAW_AREA_TOP_LEFT_Y 10
#define DRAW_AREA_SQUARE_WIDTH 40
#define DRAW_AREA_SQUARE_HEIGHT 40
//IMPORTANT: there should be enough identifiers for DRAW_AREA_WIDTH*DRAW_AREA_HEIGHT squares
#define ID_DRAW_AREA_1 30001
#define ID_DRAW_AREA_2 30002
#define ID_DRAW_AREA_3 30003
#define ID_DRAW_AREA_4 30004
#define ID_DRAW_AREA_5 30005
#define ID_DRAW_AREA_6 30006
#define ID_DRAW_AREA_7 30007
#define ID_DRAW_AREA_8 30008
#define ID_DRAW_AREA_9 30009
#define ID_DRAW_AREA_10 30010
#define ID_DRAW_AREA_11 30011
#define ID_DRAW_AREA_12 30012
#define ID_DRAW_AREA_13 30013
#define ID_DRAW_AREA_14 30014
#define ID_DRAW_AREA_15 30015
#define ID_DRAW_AREA_16 30016
#define ID_DRAW_AREA_17 30017
#define ID_DRAW_AREA_18 30018
#define ID_DRAW_AREA_19 30019
#define ID_DRAW_AREA_20 30020
#define ID_DRAW_AREA_21 30021
#define ID_DRAW_AREA_22 30022
#define ID_DRAW_AREA_23 30023
#define ID_DRAW_AREA_24 30024
#define ID_DRAW_AREA_25 30025
#define ID_DRAW_AREA_26 30026
#define ID_DRAW_AREA_27 30027
#define ID_DRAW_AREA_28 30028
#define ID_DRAW_AREA_29 30029
#define ID_DRAW_AREA_30 30030
#define ID_DRAW_AREA_31 30031
#define ID_DRAW_AREA_32 30032
#define ID_DRAW_AREA_33 30033
#define ID_DRAW_AREA_34 30034
#define ID_DRAW_AREA_35 30035
#define ID_DRAW_AREA_36 30036
#define ID_DRAW_AREA_37 30037
#define ID_DRAW_AREA_38 30038
#define ID_DRAW_AREA_39 30039
#define ID_DRAW_AREA_40 30040
#define ID_DRAW_AREA_41 30041
#define ID_DRAW_AREA_42 30042
#define ID_DRAW_AREA_43 30043
#define ID_DRAW_AREA_44 30044
#define ID_DRAW_AREA_45 30045
#define ID_DRAW_AREA_46 30046
#define ID_DRAW_AREA_47 30047
#define ID_DRAW_AREA_48 30048
#define ID_DRAW_AREA_49 30049
#define ID_DRAW_AREA_50 30050
#define ID_DRAW_AREA_51 30051
#define ID_DRAW_AREA_52 30052
#define ID_DRAW_AREA_53 30053
#define ID_DRAW_AREA_54 30054
#define ID_DRAW_AREA_55 30055
#define ID_DRAW_AREA_56 30056
#define ID_DRAW_AREA_57 30057
#define ID_DRAW_AREA_58 30058
#define ID_DRAW_AREA_59 30059
#define ID_DRAW_AREA_60 30060
#define ID_DRAW_AREA_61 30061
#define ID_DRAW_AREA_62 30062
#define ID_DRAW_AREA_63 30063
#define ID_DRAW_AREA_64 30064
#define ID_DRAW_AREA_65 30065
#define ID_DRAW_AREA_66 30066
#define ID_DRAW_AREA_67 30067
#define ID_DRAW_AREA_68 30068
#define ID_DRAW_AREA_69 30069
#define ID_DRAW_AREA_70 30070
#define ID_DRAW_AREA_71 30071
#define ID_DRAW_AREA_72 30072
#define ID_DRAW_AREA_73 30073
#define ID_DRAW_AREA_74 30074
#define ID_DRAW_AREA_75 30075
#define ID_DRAW_AREA_76 30076
#define ID_DRAW_AREA_77 30077
#define ID_DRAW_AREA_78 30078
#define ID_DRAW_AREA_79 30079
#define ID_DRAW_AREA_80 30080
#define ID_DRAW_AREA_81 30081
#define ID_DRAW_AREA_82 30082
#define ID_DRAW_AREA_83 30083
#define ID_DRAW_AREA_84 30084
#define ID_DRAW_AREA_85 30085
#define ID_DRAW_AREA_86 30086
#define ID_DRAW_AREA_87 30087
#define ID_DRAW_AREA_88 30088
#define ID_DRAW_AREA_89 30089
#define ID_DRAW_AREA_90 30090
#define ID_DRAW_AREA_91 30091
#define ID_DRAW_AREA_92 30092
#define ID_DRAW_AREA_93 30093
#define ID_DRAW_AREA_94 30094
#define ID_DRAW_AREA_95 30095
#define ID_DRAW_AREA_96 30096
#define ID_DRAW_AREA_97 30097
#define ID_DRAW_AREA_98 30098
#define ID_DRAW_AREA_99 30099
#define ID_DRAW_AREA_100 30100
#define ID_DRAW_AREA_101 30101
#define ID_DRAW_AREA_102 30102
#define ID_DRAW_AREA_103 30103
#define ID_DRAW_AREA_104 30104
#define ID_DRAW_AREA_105 30105
#define ID_DRAW_AREA_106 30106
#define ID_DRAW_AREA_107 30107
#define ID_DRAW_AREA_108 30108
#define ID_DRAW_AREA_109 30109
#define ID_DRAW_AREA_110 30110
#define ID_DRAW_AREA_111 30111
#define ID_DRAW_AREA_112 30112
#define ID_DRAW_AREA_113 30113
#define ID_DRAW_AREA_114 30114
#define ID_DRAW_AREA_115 30115
#define ID_DRAW_AREA_116 30116
#define ID_DRAW_AREA_117 30117
#define ID_DRAW_AREA_118 30118
#define ID_DRAW_AREA_119 30119
#define ID_DRAW_AREA_120 30120
#define ID_DRAW_AREA_121 30121
#define ID_DRAW_AREA_122 30122
#define ID_DRAW_AREA_123 30123
#define ID_DRAW_AREA_124 30124
#define ID_DRAW_AREA_125 30125
#define ID_DRAW_AREA_126 30126
#define ID_DRAW_AREA_127 30127
#define ID_DRAW_AREA_128 30128
#define ID_DRAW_AREA_129 30129
#define ID_DRAW_AREA_130 30130
#define ID_DRAW_AREA_131 30131
#define ID_DRAW_AREA_132 30132
#define ID_DRAW_AREA_133 30133
#define ID_DRAW_AREA_134 30134
#define ID_DRAW_AREA_135 30135
#define ID_DRAW_AREA_136 30136
#define ID_DRAW_AREA_137 30137
#define ID_DRAW_AREA_138 30138
#define ID_DRAW_AREA_139 30139
#define ID_DRAW_AREA_140 30140
#define ID_DRAW_AREA_141 30141
#define ID_DRAW_AREA_142 30142
#define ID_DRAW_AREA_143 30143
#define ID_DRAW_AREA_144 30144
#define ID_DRAW_AREA_145 30145
#define ID_DRAW_AREA_146 30146
#define ID_DRAW_AREA_147 30147
#define ID_DRAW_AREA_148 30148
#define ID_DRAW_AREA_149 30149
#define ID_DRAW_AREA_150 30150
#define ID_DRAW_AREA_151 30151
#define ID_DRAW_AREA_152 30152
#define ID_DRAW_AREA_153 30153
#define ID_DRAW_AREA_154 30154
#define ID_DRAW_AREA_155 30155
#define ID_DRAW_AREA_156 30156
#define ID_DRAW_AREA_157 30157
#define ID_DRAW_AREA_158 30158
#define ID_DRAW_AREA_159 30159
#define ID_DRAW_AREA_160 30160
#define ID_DRAW_AREA_161 30161
#define ID_DRAW_AREA_162 30162
#define ID_DRAW_AREA_163 30163
#define ID_DRAW_AREA_164 30164
#define ID_DRAW_AREA_165 30165
#define ID_DRAW_AREA_166 30166
#define ID_DRAW_AREA_167 30167
#define ID_DRAW_AREA_168 30168
#define ID_DRAW_AREA_169 30169
#define ID_DRAW_AREA_170 30170
#define ID_DRAW_AREA_171 30171
#define ID_DRAW_AREA_172 30172
#define ID_DRAW_AREA_173 30173
#define ID_DRAW_AREA_174 30174
#define ID_DRAW_AREA_175 30175
#define ID_DRAW_AREA_176 30176
#define ID_DRAW_AREA_177 30177
#define ID_DRAW_AREA_178 30178
#define ID_DRAW_AREA_179 30179
#define ID_DRAW_AREA_180 30180
#define ID_DRAW_AREA_181 30181
#define ID_DRAW_AREA_182 30182
#define ID_DRAW_AREA_183 30183
#define ID_DRAW_AREA_184 30184
#define ID_DRAW_AREA_185 30185
#define ID_DRAW_AREA_186 30186
#define ID_DRAW_AREA_187 30187
#define ID_DRAW_AREA_188 30188
#define ID_DRAW_AREA_189 30189
#define ID_DRAW_AREA_190 30190
#define ID_DRAW_AREA_191 30191
#define ID_DRAW_AREA_192 30192
#define ID_DRAW_AREA_193 30193
#define ID_DRAW_AREA_194 30194
#define ID_DRAW_AREA_195 30195
#define ID_DRAW_AREA_196 30196
#define ID_DRAW_AREA_197 30197
#define ID_DRAW_AREA_198 30198
#define ID_DRAW_AREA_199 30199
#define ID_DRAW_AREA_200 30200
#define ID_DRAW_AREA_201 30201
#define ID_DRAW_AREA_202 30202
#define ID_DRAW_AREA_203 30203
#define ID_DRAW_AREA_204 30204
#define ID_DRAW_AREA_205 30205
#define ID_DRAW_AREA_206 30206
#define ID_DRAW_AREA_207 30207
#define ID_DRAW_AREA_208 30208
#define ID_DRAW_AREA_209 30209
#define ID_DRAW_AREA_210 30210
#define ID_DRAW_AREA_211 30211
#define ID_DRAW_AREA_212 30212
#define ID_DRAW_AREA_213 30213
#define ID_DRAW_AREA_214 30214
#define ID_DRAW_AREA_215 30215
#define ID_DRAW_AREA_216 30216
#define ID_DRAW_AREA_217 30217
#define ID_DRAW_AREA_218 30218
#define ID_DRAW_AREA_219 30219
#define ID_DRAW_AREA_220 30220
#define ID_DRAW_AREA_221 30221
#define ID_DRAW_AREA_222 30222
#define ID_DRAW_AREA_223 30223
#define ID_DRAW_AREA_224 30224
#define ID_DRAW_AREA_225 30225
#define ID_DRAW_AREA_226 30226
#define ID_DRAW_AREA_227 30227
#define ID_DRAW_AREA_228 30228
#define ID_DRAW_AREA_229 30229
#define ID_DRAW_AREA_230 30230
#define ID_DRAW_AREA_231 30231
#define ID_DRAW_AREA_232 30232
#define ID_DRAW_AREA_233 30233
#define ID_DRAW_AREA_234 30234
#define ID_DRAW_AREA_235 30235
#define ID_DRAW_AREA_236 30236
#define ID_DRAW_AREA_237 30237
#define ID_DRAW_AREA_238 30238
#define ID_DRAW_AREA_239 30239
#define ID_DRAW_AREA_240 30240
#define ID_DRAW_AREA_241 30241
#define ID_DRAW_AREA_242 30242
#define ID_DRAW_AREA_243 30243
#define ID_DRAW_AREA_244 30244
#define ID_DRAW_AREA_245 30245
#define ID_DRAW_AREA_246 30246
#define ID_DRAW_AREA_247 30247
#define ID_DRAW_AREA_248 30248
#define ID_DRAW_AREA_249 30249
#define ID_DRAW_AREA_250 30250
#define ID_DRAW_AREA_251 30251
#define ID_DRAW_AREA_252 30252
#define ID_DRAW_AREA_253 30253
#define ID_DRAW_AREA_254 30254
#define ID_DRAW_AREA_255 30255
#define ID_DRAW_AREA_256 30256

#endif // USER_INTERFACE_H_INCLUDED
