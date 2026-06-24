#include "evaluate.hpp"

static constexpr int32_t piece_values[5] = {S(304, 197), S(1081, 785), S(1176, 931), S(1825, 1828), S(4149, 3329)};
static constexpr int32_t PSQT_PAWN[48] = {S(-50, 351), S(72, 285), S(49, 332), S(40, 290), S(79, 332), S(320, 254), S(317, 217), S(70, 230), S(10, 300), S(60, 291), S(148, 242), S(119, 266), S(192, 273), S(212, 250), S(302, 203), S(128, 226), S(-4, 357), S(72, 320), S(146, 254), S(255, 206), S(278, 218), S(225, 227), S(171, 256), S(54, 269), S(76, 409), S(179, 344), S(219, 275), S(312, 169), S(319, 208), S(277, 239), S(231, 303), S(80, 332), S(170, 505), S(214, 458), S(334, 332), S(349, 148), S(581, 59), S(688, 166), S(356, 342), S(164, 409), S(309, 689), S(529, 589), S(187, 521), S(442, 339), S(246, 428), S(562, 326), S(-80, 604), S(-378, 791)};
static constexpr int32_t PSQT_KNIGHT[64] = {S(221, 690), S(823, 445), S(592, 653), S(726, 689), S(856, 627), S(784, 648), S(845, 448), S(819, 371), S(762, 559), S(603, 680), S(871, 712), S(924, 729), S(936, 743), S(1042, 652), S(842, 633), S(841, 495), S(811, 625), S(888, 746), S(1013, 746), S(1000, 836), S(1050, 814), S(1048, 732), S(1084, 633), S(854, 629), S(864, 670), S(967, 727), S(1021, 850), S(1009, 903), S(1093, 846), S(1053, 853), S(1050, 783), S(895, 654), S(859, 664), S(1036, 775), S(1040, 884), S(1247, 861), S(1160, 870), S(1349, 786), S(1046, 788), S(1057, 648), S(662, 634), S(1280, 628), S(1141, 803), S(1301, 787), S(1453, 689), S(1673, 657), S(1382, 608), S(1219, 485), S(469, 637), S(649, 731), S(1317, 591), S(1086, 753), S(1053, 704), S(1293, 579), S(900, 625), S(842, 471), S(-177, 511), S(334, 575), S(673, 709), S(686, 600), S(1364, 535), S(314, 644), S(798, 421), S(235, 285)};
static constexpr int32_t PSQT_BISHOP[64] = {S(642, 586), S(839, 659), S(796, 542), S(740, 671), S(780, 647), S(806, 586), S(631, 701), S(706, 623), S(889, 623), S(964, 589), S(964, 653), S(871, 689), S(922, 701), S(973, 649), S(1062, 587), S(899, 530), S(870, 638), S(961, 681), S(947, 748), S(961, 754), S(946, 769), S(1029, 694), S(968, 666), S(913, 619), S(825, 665), S(947, 707), S(946, 768), S(1010, 802), S(1064, 734), S(943, 745), S(921, 676), S(881, 645), S(852, 675), S(912, 738), S(981, 756), S(1141, 750), S(1081, 769), S(1076, 730), S(919, 695), S(848, 699), S(778, 700), S(1064, 641), S(1108, 684), S(1079, 679), S(1065, 670), S(1220, 684), S(1056, 695), S(866, 710), S(696, 661), S(901, 676), S(746, 738), S(696, 671), S(1040, 672), S(1164, 634), S(940, 673), S(595, 637), S(647, 628), S(843, 585), S(195, 702), S(471, 690), S(606, 701), S(586, 677), S(814, 641), S(808, 562)};
static constexpr int32_t PSQT_ROOK[64] = {S(815, 1070), S(859, 1117), S(935, 1122), S(987, 1104), S(1001, 1075), S(916, 1068), S(769, 1123), S(815, 967), S(666, 1102), S(860, 1067), S(823, 1102), S(871, 1114), S(927, 1043), S(973, 1036), S(925, 1017), S(544, 1121), S(661, 1114), S(814, 1110), S(854, 1075), S(825, 1102), S(940, 1053), S(926, 1031), S(932, 1041), S(756, 1031), S(710, 1142), S(792, 1141), S(870, 1146), S(928, 1117), S(969, 1074), S(895, 1068), S(1028, 1022), S(801, 1060), S(809, 1133), S(890, 1115), S(982, 1158), S(1095, 1083), S(1072, 1089), S(1141, 1070), S(952, 1066), S(848, 1118), S(915, 1133), S(1069, 1117), S(1106, 1103), S(1148, 1102), S(1024, 1102), S(1273, 1017), S(1384, 995), S(1072, 1038), S(1171, 1109), S(1155, 1130), S(1383, 1084), S(1408, 1079), S(1503, 992), S(1466, 1025), S(1114, 1114), S(1240, 1070), S(1177, 1130), S(1276, 1098), S(1138, 1169), S(1422, 1089), S(1394, 1097), S(968, 1161), S(1058, 1136), S(1162, 1107)};
static constexpr int32_t PSQT_QUEEN[64] = {S(1884, 1673), S(1822, 1680), S(1866, 1730), S(1979, 1543), S(1815, 1844), S(1749, 1689), S(1709, 1778), S(1625, 1624), S(1707, 1777), S(1863, 1734), S(1988, 1660), S(1926, 1765), S(1962, 1764), S(2007, 1691), S(1912, 1619), S(1935, 1663), S(1802, 1852), S(1940, 1660), S(1844, 1976), S(1902, 1891), S(1877, 1952), S(1915, 1971), S(1979, 1921), S(1920, 1927), S(1867, 1743), S(1731, 2086), S(1849, 1994), S(1842, 2156), S(1874, 2071), S(1894, 2056), S(1912, 2090), S(1895, 1997), S(1723, 1951), S(1752, 2037), S(1809, 2027), S(1812, 2149), S(1873, 2234), S(1957, 2133), S(1876, 2269), S(1891, 2115), S(1856, 1741), S(1829, 1890), S(1942, 1897), S(1881, 2204), S(2025, 2175), S(2308, 1974), S(2195, 1959), S(2237, 1854), S(1772, 1776), S(1648, 2031), S(1847, 2079), S(1885, 2147), S(1722, 2304), S(2276, 1924), S(2027, 2044), S(2234, 1807), S(1697, 1864), S(1846, 2052), S(1956, 2044), S(1922, 2078), S(2547, 1762), S(2568, 1688), S(2243, 1795), S(2167, 1973)};
static constexpr int32_t PSQT_KING[64] = {S(-27, -327), S(277, -250), S(94, -134), S(-406, -8), S(23, -165), S(-206, -38), S(204, -188), S(191, -327), S(91, -172), S(109, -71), S(-88, 60), S(-431, 133), S(-317, 135), S(-165, 78), S(96, -29), S(145, -133), S(66, -122), S(-26, -4), S(-196, 105), S(-389, 180), S(-376, 195), S(-317, 159), S(-68, 57), S(-176, -17), S(-366, -57), S(115, -40), S(-314, 171), S(-595, 238), S(-619, 255), S(-392, 198), S(-352, 116), S(-446, 8), S(128, -103), S(-60, 111), S(118, 111), S(-254, 188), S(-220, 173), S(-189, 209), S(20, 140), S(-369, 71), S(424, -47), S(391, 34), S(500, 32), S(112, 62), S(194, 52), S(604, 148), S(661, 118), S(-52, 54), S(1039, -283), S(354, 16), S(145, 36), S(566, -19), S(215, 54), S(199, 153), S(-162, 136), S(-633, 161), S(-328, -399), S(927, -369), S(833, -274), S(444, -204), S(-674, 41), S(-392, 143), S(271, -44), S(291, -137)};
static constexpr int32_t ISOLATED_PAWN = S(-97, -46);
static constexpr int32_t PASSED_PAWNS[6] = {S(54, 15), S(11, 46), S(-38, 163), S(33, 292), S(49, 639), S(407, 770)};
static constexpr int32_t BISHOP_PAIR = S(167, 261);
static constexpr int32_t DOUBLED_PAWN = S(-3, -82);

int32_t eval_material(const chess::Board& board, const chess::Color color) {
    int32_t score = 0;

    chess::Bitboard our_pawns   = board.pieces(chess::PieceType::PAWN, color);
    chess::Bitboard our_knights = board.pieces(chess::PieceType::KNIGHT, color);
    chess::Bitboard our_bishops = board.pieces(chess::PieceType::BISHOP, color);
    chess::Bitboard our_rooks   = board.pieces(chess::PieceType::ROOK, color);
    chess::Bitboard our_queens  = board.pieces(chess::PieceType::QUEEN, color);

    // material score
    score += piece_values[0] * our_pawns.count();
    score += piece_values[1] * our_knights.count();
    score += piece_values[2] * our_bishops.count();
    score += piece_values[3] * our_rooks.count();
    score += piece_values[4] * our_queens.count();

    return score;
}

int32_t eval_psqt(const chess::Board& board, const chess::Color color) {
    int32_t score = 0;

    chess::Bitboard our_pawns   = board.pieces(chess::PieceType::PAWN, color);
    chess::Bitboard our_knights = board.pieces(chess::PieceType::KNIGHT, color);
    chess::Bitboard our_bishops = board.pieces(chess::PieceType::BISHOP, color);
    chess::Bitboard our_rooks   = board.pieces(chess::PieceType::ROOK, color);
    chess::Bitboard our_queens  = board.pieces(chess::PieceType::QUEEN, color);
    chess::Bitboard our_king    = board.pieces(chess::PieceType::KING, color);

    while (our_pawns) {
        chess::Square sq = our_pawns.pop();
        score += PSQT_PAWN[sq.relative_square(color).index() - 8];
    }

    while (our_knights) {
        chess::Square sq = our_knights.pop();
        score += PSQT_KNIGHT[sq.relative_square(color).index()];
    }

    while (our_bishops) {
        chess::Square sq = our_bishops.pop();
        score += PSQT_BISHOP[sq.relative_square(color).index()];
    }

    while (our_rooks) {
        chess::Square sq = our_rooks.pop();
        score += PSQT_ROOK[sq.relative_square(color).index()];
    }

    while (our_queens) {
        chess::Square sq = our_queens.pop();
        score += PSQT_QUEEN[sq.relative_square(color).index()];
    }

    while (our_king) {
        chess::Square sq = our_king.pop();
        score += PSQT_KING[sq.relative_square(color).index()];
    }

    return score;
}

template <chess::Color::underlying color>
int32_t eval_isolated_pawns(const chess::Board& board) {
    int32_t score = 0;

    chess::Bitboard our_pawns   = board.pieces(chess::PieceType::PAWN, color);

    chess::Bitboard isolated_pawns = ~FillBackward(FillForward(PawnAttacks<color>(our_pawns))) & our_pawns;
    score += ISOLATED_PAWN * isolated_pawns.count();

    return score;
}

template <chess::Color::underlying color>
int32_t eval_passed_pawns(const chess::Board& board) {
    int32_t score = 0;

    chess::Bitboard our_pawns   = board.pieces(chess::PieceType::PAWN, color);
    chess::Bitboard opp_pawns   = board.pieces(chess::PieceType::PAWN, ~color);

    chess::Bitboard passed_pawns;
    if constexpr (color == chess::Color::WHITE) {
        passed_pawns = ~FillBackward(PawnAttacks<chess::Color::BLACK>(opp_pawns) | opp_pawns >> 8) & our_pawns;
    } else {
        passed_pawns = ~FillForward(PawnAttacks<chess::Color::WHITE>(opp_pawns) | opp_pawns << 8) & our_pawns;
    }
    
    while (passed_pawns) {
        chess::Square sq = passed_pawns.pop();
        score += PASSED_PAWNS[sq.relative_square(color).rank() - 1];
    }

    return score;
}

template <chess::Color::underlying color>
int32_t eval_doubled_pawns(const chess::Board& board) {
    int32_t score = 0;

    chess::Bitboard our_pawns = board.pieces(chess::PieceType::PAWN, color);
    chess::Bitboard mask;
    
    if constexpr (color == chess::Color::WHITE) {
        mask = FillForward(our_pawns << 8) & our_pawns;
    } else {
        mask = FillBackward(our_pawns >> 8) & our_pawns;
    }

    score += DOUBLED_PAWN * mask.count();

    return score;
}

template <chess::Color::underlying color>
int32_t eval_pawn_structures(const chess::Board& board) {
    int32_t score = 0;

    score += eval_isolated_pawns<color>(board);
    score += eval_passed_pawns<color>(board);
    score += eval_doubled_pawns<color>(board);

    return score;
}

int32_t eval_bishop_pair(const chess::Board& board, const chess::Color color) {
    return BISHOP_PAIR * (board.pieces(chess::PieceType::BISHOP, color).count() >= 2);
}

int phase_accumulator(const chess::Board& board) {
    int phase = 0;
    
    chess::Bitboard all_bishops = board.pieces(chess::PieceType::BISHOP);
    chess::Bitboard all_knights = board.pieces(chess::PieceType::KNIGHT);
    chess::Bitboard all_rooks   = board.pieces(chess::PieceType::ROOK);
    chess::Bitboard all_queens  = board.pieces(chess::PieceType::QUEEN);

    phase += 1 * all_bishops.count();
    phase += 1 * all_knights.count();
    phase += 2 * all_rooks.count();
    phase += 4 * all_queens.count();

    return phase;
}

template <chess::Color::underlying color>
int32_t eval_all(const chess::Board& board) {
    int32_t packed_score = 0;

    packed_score += eval_material(board, color);
    packed_score += eval_psqt(board, color);
    packed_score += eval_pawn_structures<color>(board);
    packed_score += eval_bishop_pair(board, color);

    return packed_score;
}

int16_t evaluate(const chess::Board& board) {
    int phase = phase_accumulator(board);

    int32_t packed_score = eval_all<chess::Color::WHITE>(board) - eval_all<chess::Color::BLACK>(board);

    int16_t mg = mg_score(packed_score);
    int16_t eg = eg_score(packed_score);

    phase = std::min(phase, 24);

    int16_t score = (mg * phase + eg * (24 - phase)) / 24;

    return board.sideToMove() == chess::Color::WHITE ? score : -score;
}