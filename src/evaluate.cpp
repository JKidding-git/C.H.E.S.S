#include "evaluate.hpp"

static constexpr int32_t piece_values[5] = {S(302, 177), S(1080, 788), S(1173, 933), S(1821, 1833), S(4142, 3338)};
static constexpr int32_t PSQT_PAWN[48] = {S(-47, 354), S(71, 278), S(51, 330), S(41, 288), S(80, 329), S(320, 251), S(316, 208), S(74, 232), S(12, 304), S(58, 286), S(148, 242), S(120, 265), S(193, 274), S(210, 246), S(301, 195), S(132, 229), S(-2, 361), S(71, 315), S(147, 257), S(257, 213), S(279, 222), S(224, 224), S(170, 248), S(57, 273), S(79, 413), S(177, 338), S(218, 275), S(311, 170), S(318, 206), S(275, 233), S(229, 293), S(83, 335), S(173, 509), S(210, 453), S(333, 329), S(349, 148), S(579, 55), S(680, 159), S(353, 330), S(167, 409), S(316, 697), S(521, 592), S(190, 524), S(448, 346), S(246, 430), S(549, 324), S(-87, 599), S(-372, 795)};
static constexpr int32_t PSQT_KNIGHT[64] = {S(219, 691), S(824, 446), S(592, 655), S(725, 689), S(855, 628), S(783, 648), S(845, 447), S(820, 369), S(761, 561), S(602, 680), S(870, 712), S(924, 728), S(936, 743), S(1042, 652), S(841, 632), S(841, 494), S(811, 625), S(887, 747), S(1013, 746), S(1000, 835), S(1050, 812), S(1048, 733), S(1084, 634), S(854, 629), S(864, 668), S(967, 726), S(1021, 849), S(1009, 902), S(1093, 845), S(1053, 854), S(1051, 784), S(895, 655), S(859, 663), S(1036, 775), S(1040, 884), S(1247, 862), S(1160, 870), S(1349, 786), S(1046, 789), S(1055, 649), S(663, 630), S(1280, 628), S(1142, 803), S(1300, 788), S(1455, 689), S(1673, 657), S(1381, 608), S(1218, 487), S(468, 639), S(649, 731), S(1318, 590), S(1086, 752), S(1053, 705), S(1293, 581), S(898, 625), S(840, 475), S(-176, 511), S(332, 578), S(673, 709), S(686, 602), S(1365, 534), S(317, 644), S(803, 418), S(238, 283)};
static constexpr int32_t PSQT_BISHOP[64] = {S(641, 586), S(839, 659), S(796, 541), S(740, 670), S(779, 648), S(807, 585), S(630, 701), S(705, 620), S(889, 622), S(964, 588), S(964, 652), S(871, 688), S(922, 701), S(973, 649), S(1062, 587), S(899, 529), S(869, 636), S(960, 680), S(947, 748), S(961, 752), S(946, 769), S(1029, 694), S(968, 665), S(914, 618), S(824, 664), S(948, 706), S(946, 767), S(1010, 802), S(1064, 734), S(942, 745), S(922, 677), S(881, 647), S(851, 676), S(911, 739), S(980, 756), S(1142, 750), S(1082, 768), S(1075, 733), S(918, 696), S(848, 702), S(778, 701), S(1065, 641), S(1107, 685), S(1079, 679), S(1067, 671), S(1222, 685), S(1056, 695), S(866, 711), S(696, 660), S(900, 676), S(747, 737), S(695, 671), S(1042, 670), S(1163, 637), S(938, 674), S(594, 638), S(648, 630), S(845, 586), S(196, 702), S(474, 690), S(603, 705), S(588, 677), S(812, 641), S(807, 560)};
static constexpr int32_t PSQT_ROOK[64] = {S(816, 1070), S(860, 1116), S(935, 1121), S(988, 1104), S(1002, 1074), S(917, 1066), S(769, 1120), S(816, 966), S(667, 1103), S(861, 1066), S(823, 1102), S(872, 1114), S(928, 1042), S(974, 1035), S(926, 1016), S(545, 1120), S(663, 1113), S(815, 1111), S(854, 1074), S(827, 1102), S(941, 1052), S(926, 1030), S(932, 1041), S(757, 1032), S(710, 1142), S(792, 1141), S(869, 1146), S(927, 1119), S(969, 1074), S(892, 1069), S(1028, 1024), S(801, 1060), S(809, 1134), S(891, 1115), S(983, 1157), S(1098, 1082), S(1074, 1089), S(1137, 1071), S(949, 1067), S(847, 1119), S(915, 1133), S(1069, 1117), S(1106, 1103), S(1149, 1102), S(1023, 1104), S(1271, 1018), S(1382, 996), S(1066, 1041), S(1173, 1108), S(1155, 1130), S(1383, 1084), S(1409, 1079), S(1504, 992), S(1464, 1027), S(1110, 1114), S(1237, 1072), S(1178, 1130), S(1274, 1099), S(1141, 1168), S(1418, 1090), S(1394, 1098), S(968, 1161), S(1056, 1137), S(1164, 1107)};
static constexpr int32_t PSQT_QUEEN[64] = {S(1884, 1673), S(1822, 1681), S(1866, 1728), S(1979, 1542), S(1815, 1843), S(1749, 1691), S(1710, 1776), S(1623, 1624), S(1707, 1778), S(1862, 1735), S(1987, 1660), S(1926, 1764), S(1961, 1765), S(2007, 1689), S(1911, 1621), S(1937, 1659), S(1802, 1854), S(1940, 1661), S(1844, 1975), S(1902, 1891), S(1876, 1954), S(1915, 1971), S(1978, 1921), S(1919, 1930), S(1867, 1742), S(1731, 2087), S(1848, 1994), S(1842, 2157), S(1874, 2071), S(1893, 2059), S(1911, 2091), S(1895, 2001), S(1723, 1951), S(1752, 2036), S(1809, 2025), S(1812, 2148), S(1873, 2234), S(1957, 2135), S(1876, 2270), S(1891, 2116), S(1856, 1740), S(1830, 1889), S(1942, 1897), S(1881, 2205), S(2025, 2176), S(2307, 1977), S(2195, 1958), S(2236, 1855), S(1773, 1776), S(1647, 2030), S(1846, 2080), S(1885, 2146), S(1726, 2301), S(2275, 1925), S(2027, 2042), S(2233, 1808), S(1696, 1864), S(1848, 2052), S(1955, 2044), S(1923, 2077), S(2547, 1762), S(2569, 1688), S(2246, 1796), S(2168, 1971)};
static constexpr int32_t PSQT_KING[64] = {S(-24, -328), S(280, -251), S(97, -136), S(-403, -10), S(26, -167), S(-203, -40), S(207, -188), S(194, -328), S(91, -173), S(112, -72), S(-85, 57), S(-429, 131), S(-316, 133), S(-162, 76), S(99, -31), S(148, -135), S(63, -122), S(-24, -5), S(-194, 102), S(-389, 178), S(-373, 193), S(-316, 157), S(-66, 55), S(-175, -19), S(-373, -54), S(117, -41), S(-314, 170), S(-592, 237), S(-621, 254), S(-395, 197), S(-353, 115), S(-447, 8), S(130, -102), S(-57, 111), S(114, 112), S(-252, 187), S(-214, 172), S(-192, 210), S(12, 142), S(-374, 73), S(421, -47), S(394, 34), S(497, 33), S(119, 60), S(192, 53), S(593, 152), S(653, 122), S(-62, 58), S(1037, -284), S(351, 17), S(144, 36), S(556, -16), S(219, 54), S(199, 154), S(-163, 138), S(-631, 163), S(-322, -399), S(919, -365), S(830, -272), S(441, -201), S(-669, 44), S(-385, 144), S(269, -40), S(290, -135)};
static constexpr int32_t ISOLATED_PAWN = S(-101, -61);
static constexpr int32_t PASSED_PAWNS[6] = {S(57, 37), S(13, 69), S(-39, 185), S(31, 313), S(48, 661), S(401, 788)};
static constexpr int32_t BISHOP_PAIR = S(168, 259);

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
    packed_score += eval_isolated_pawns<color>(board);
    packed_score += eval_passed_pawns<color>(board);
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