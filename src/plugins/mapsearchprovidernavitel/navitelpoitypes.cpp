#include <QDebug>
#include <QDir>
#include <definitions/resources.h>
#include "mapsearchprovidernavitel.h"

void MapSearchProviderNavitel::fullNavitelTypes()
{    
	FNavitelTypes.insert(2, QString("highway:secondary"));
	FNavitelTypes.insert(3, QString("highway:road"));
	FNavitelTypes.insert(5, QString("highway:tertiary"));
	FNavitelTypes.insert(6, QString("highway:tertiary"));
	FNavitelTypes.insert(7, QString("highway:primary"));
  FNavitelTypes.insert(8, QString("highway:primary"));
  FNavitelTypes.insert(9, QString());
  FNavitelTypes.insert(10, QString());
  FNavitelTypes.insert(11, QString("highway:road"));
  FNavitelTypes.insert(12, QString("highway:tertiary"));
  FNavitelTypes.insert(13, QString("highway:primary"));
  FNavitelTypes.insert(15, QString("highway:primary"));
  FNavitelTypes.insert(18, QString("highway:primary"));
  FNavitelTypes.insert(20, QString("highway:road"));
  FNavitelTypes.insert(24, QString("highway:primary"));
	FNavitelTypes.insert(27, QString("highway:primary"));
	FNavitelTypes.insert(29, QString("highway:secondary"));
  FNavitelTypes.insert(30, QString("place:village"));
  FNavitelTypes.insert(31, QString("place:village"));
  FNavitelTypes.insert(32, QString("place:hamlet"));
  FNavitelTypes.insert(33, QString("place:city"));
  FNavitelTypes.insert(34, QString("place:administrative"));
  FNavitelTypes.insert(35, QString());
  FNavitelTypes.insert(37, QString());
  FNavitelTypes.insert(38, QString());
  FNavitelTypes.insert(39, QString());
  FNavitelTypes.insert(40, QString());
  FNavitelTypes.insert(41, QString("place:village"));
  FNavitelTypes.insert(42, QString());
  FNavitelTypes.insert(44, QString("place:village"));
  FNavitelTypes.insert(46, QString());
  FNavitelTypes.insert(47, QString("place:village"));
  FNavitelTypes.insert(48, QString());
  FNavitelTypes.insert(49, QString());
  FNavitelTypes.insert(51, QString());
  FNavitelTypes.insert(52, QString());
  FNavitelTypes.insert(53, QString());
  FNavitelTypes.insert(54, QString());
  FNavitelTypes.insert(55, QString());
  FNavitelTypes.insert(56, QString());
  FNavitelTypes.insert(57, QString());
  FNavitelTypes.insert(60, QString());
  FNavitelTypes.insert(58, QString("place:administrative"));
  FNavitelTypes.insert(59, QString("place:administrative"));
  FNavitelTypes.insert(61, QString("place:town"));
  FNavitelTypes.insert(62, QString());
  FNavitelTypes.insert(63, QString());
  FNavitelTypes.insert(65, QString());
  FNavitelTypes.insert(66, QString("place:administrative"));
  FNavitelTypes.insert(67, QString());
  FNavitelTypes.insert(68, QString());
  FNavitelTypes.insert(69, QString());
  FNavitelTypes.insert(71, QString("place:city"));
  FNavitelTypes.insert(72, QString("place:city"));
  FNavitelTypes.insert(75, QString());
  FNavitelTypes.insert(76, QString());
  FNavitelTypes.insert(77, QString());
  FNavitelTypes.insert(78, QString());
  FNavitelTypes.insert(79, QString());
  FNavitelTypes.insert(81, QString("highway::secondary"));
  FNavitelTypes.insert(80, QString("highway::secondary"));
  FNavitelTypes.insert(82, QString("highway::secondary"));
  FNavitelTypes.insert(83, QString());
  FNavitelTypes.insert(84, QString("highway::primary"));
  FNavitelTypes.insert(85, QString("highway::primary"));
  FNavitelTypes.insert(86, QString("highway::secondary"));
  FNavitelTypes.insert(87, QString("highway::tertiary"));
  FNavitelTypes.insert(88, QString("highway::secondary"));
  FNavitelTypes.insert(89, QString("public_transport:station"));
  FNavitelTypes.insert(90, QString("highway:bus_stop"));
  FNavitelTypes.insert(91, QString("railway:tram_stop"));
  FNavitelTypes.insert(92, QString());
  FNavitelTypes.insert(93, QString("railway:subway"));
  FNavitelTypes.insert(94, QString("railway:station"));
  FNavitelTypes.insert(95, QString("railway:station"));
  FNavitelTypes.insert(96, QString());
  FNavitelTypes.insert(97, QString("aerialway:station"));
  FNavitelTypes.insert(98, QString());
  FNavitelTypes.insert(99, QString());
  FNavitelTypes.insert(100, QString("waterway:ferry"));
  FNavitelTypes.insert(101, QString("religion:church"));
  FNavitelTypes.insert(102, QString("religion:church"));
  FNavitelTypes.insert(103, QString("religion:church"));
  FNavitelTypes.insert(104, QString("religion:muslim"));
  FNavitelTypes.insert(105, QString("religion:jewish"));
  FNavitelTypes.insert(106, QString("religion:shinto"));
  FNavitelTypes.insert(107, QString());
  FNavitelTypes.insert(108, QString());
  FNavitelTypes.insert(109, QString());
  FNavitelTypes.insert(110, QString());
  FNavitelTypes.insert(111, QString());
  FNavitelTypes.insert(112, QString());
  FNavitelTypes.insert(113, QString());
  FNavitelTypes.insert(114, QString());
  FNavitelTypes.insert(115, QString("education:kindergarten"));
  FNavitelTypes.insert(116, QString("education:school"));
  FNavitelTypes.insert(117, QString("education:college"));
  FNavitelTypes.insert(118, QString("education:university"));
  FNavitelTypes.insert(119, QString("education:college"));
  FNavitelTypes.insert(120, QString("money:atm"));
  FNavitelTypes.insert(121, QString());
  FNavitelTypes.insert(122, QString("leisure:sauna"));
  FNavitelTypes.insert(123, QString("shop:laundry"));
  FNavitelTypes.insert(124, QString());
  FNavitelTypes.insert(125, QString("shop:tailor"));
  FNavitelTypes.insert(126, QString());
  FNavitelTypes.insert(127, QString());
  FNavitelTypes.insert(128, QString());
  FNavitelTypes.insert(129, QString());
  FNavitelTypes.insert(130, QString());
  FNavitelTypes.insert(131, QString());
  FNavitelTypes.insert(132, QString());
  FNavitelTypes.insert(133, QString());
	FNavitelTypes.insert(134, QString());
	FNavitelTypes.insert(135, QString("information"));
	FNavitelTypes.insert(136, QString());
	FNavitelTypes.insert(137, QString());
	FNavitelTypes.insert(138, QString("shop:travel_agency"));
  FNavitelTypes.insert(139, QString());
  FNavitelTypes.insert(140, QString("misc:wifi"));
  FNavitelTypes.insert(141, QString("food:cafe"));
  FNavitelTypes.insert(142, QString("place:marketplace"));
  FNavitelTypes.insert(143, QString("shop:shoes"));
  FNavitelTypes.insert(144, QString("shop"));
  FNavitelTypes.insert(145, QString("shop:chemist"));
  FNavitelTypes.insert(146, QString("shop:electronics"));
  FNavitelTypes.insert(147, QString("shop:mobile_phone"));
  FNavitelTypes.insert(148, QString("shop:outdoor"));
  FNavitelTypes.insert(149, QString("shop:weapons"));
  FNavitelTypes.insert(150, QString("shop:gift"));
  FNavitelTypes.insert(151, QString("shop:florist"));
  FNavitelTypes.insert(152, QString("shop:jewelry"));
  FNavitelTypes.insert(153, QString("shop:pet"));
  FNavitelTypes.insert(154, QString("health:dentist"));
  FNavitelTypes.insert(155, QString("health:veterinary"));
  FNavitelTypes.insert(156, QString("leisure"));
  FNavitelTypes.insert(157, QString());
  FNavitelTypes.insert(158, QString("food"));
  FNavitelTypes.insert(159, QString("food:restaurant"));
  FNavitelTypes.insert(160, QString("food:restaurant"));
  FNavitelTypes.insert(161, QString("food:restaurant"));
  FNavitelTypes.insert(162, QString("food:restaurant"));
  FNavitelTypes.insert(163, QString("food:restaurant"));
  FNavitelTypes.insert(164, QString("food:restaurant"));
  FNavitelTypes.insert(165, QString("food:fast_food"));
  FNavitelTypes.insert(166, QString("food:restaurant"));
  FNavitelTypes.insert(167, QString("food:restaurant"));
  FNavitelTypes.insert(168, QString("food:fast_food"));
  FNavitelTypes.insert(169, QString("food:restaurant"));
  FNavitelTypes.insert(170, QString("food:restaurant"));
  FNavitelTypes.insert(171, QString("shop:confectionery"));
  FNavitelTypes.insert(172, QString("food:cafe"));
  FNavitelTypes.insert(173, QString("food:restaurant"));
  FNavitelTypes.insert(174, QString("food:restaurant"));
  FNavitelTypes.insert(175, QString("food:restaurant"));
  FNavitelTypes.insert(176, QString("food"));
  FNavitelTypes.insert(177, QString("accommodation:hotel"));
  FNavitelTypes.insert(178, QString("accommodation:motel"));
  FNavitelTypes.insert(179, QString("accommodation:hotel"));
  FNavitelTypes.insert(180, QString("accommodation:camping"));
  FNavitelTypes.insert(181, QString("leisure:beach_resort"));
  FNavitelTypes.insert(182, QString("leisure:common"));
  FNavitelTypes.insert(183, QString("leisure:theme_park"));
  FNavitelTypes.insert(184, QString("sightseeing:museum"));
  FNavitelTypes.insert(185, QString("shop:library"));
  FNavitelTypes.insert(186, QString("sightseeing"));
  FNavitelTypes.insert(187, QString("education:school"));
  FNavitelTypes.insert(188, QString("leisure:garden"));
  FNavitelTypes.insert(189, QString("leisure:zoo"));
  FNavitelTypes.insert(190, QString("sport:stadium"));
  FNavitelTypes.insert(191, QString());
  FNavitelTypes.insert(192, QString());
  FNavitelTypes.insert(193, QString("religion"));
  FNavitelTypes.insert(194, QString("leisure"));
  FNavitelTypes.insert(195, QString("leisure:theater"));
  FNavitelTypes.insert(196, QString("leisure:nightclub"));
  FNavitelTypes.insert(197, QString("leisure:cinema"));
  FNavitelTypes.insert(198, QString());
  FNavitelTypes.insert(199, QString("sport:golf"));
  FNavitelTypes.insert(200, QString("sport:skiing"));
  FNavitelTypes.insert(201, QString("sport:boule"));
  FNavitelTypes.insert(202, QString("leisure:ice_rink"));
  FNavitelTypes.insert(203, QString("sport:pool"));
  FNavitelTypes.insert(204, QString("sport:gymnastics"));
  FNavitelTypes.insert(205, QString());
  FNavitelTypes.insert(206, QString("misc:commercial"));
  FNavitelTypes.insert(207, QString("shop:variety_store"));
  FNavitelTypes.insert(208, QString("shop:deli"));
  FNavitelTypes.insert(209, QString("misc:commercial"));
  FNavitelTypes.insert(210, QString("shop:mall"));
  FNavitelTypes.insert(211, QString("health:pharmacy"));
  FNavitelTypes.insert(212, QString("shop"));
  FNavitelTypes.insert(213, QString("shop:clothes"));
  FNavitelTypes.insert(214, QString("shop:garden_centre"));
  FNavitelTypes.insert(215, QString("shop:furniture"));
  FNavitelTypes.insert(216, QString("shop"));
  FNavitelTypes.insert(217, QString("shop:computer"));
  FNavitelTypes.insert(218, QString("service"));
  FNavitelTypes.insert(219, QString("vehicle:fuel"));
  FNavitelTypes.insert(220, QString("rental:car_rental"));
  FNavitelTypes.insert(221, QString("vehicle:services"));
  FNavitelTypes.insert(222, QString("aeroway:terminal"));
  FNavitelTypes.insert(223, QString("service:post_office"));
  FNavitelTypes.insert(224, QString("money:bank"));
  FNavitelTypes.insert(225, QString("shop:car"));
  FNavitelTypes.insert(226, QString());
  FNavitelTypes.insert(227, QString());
  FNavitelTypes.insert(228, QString());
  FNavitelTypes.insert(229, QString("parking"));
  FNavitelTypes.insert(230, QString());
  FNavitelTypes.insert(231, QString());
  FNavitelTypes.insert(232, QString("vehicle:car_wash"));
  FNavitelTypes.insert(233, QString());
  FNavitelTypes.insert(234, QString("shop:laundry"));
  FNavitelTypes.insert(235, QString());
  FNavitelTypes.insert(236, QString());
  FNavitelTypes.insert(237, QString());
  FNavitelTypes.insert(238, QString());
  FNavitelTypes.insert(239, QString());
  FNavitelTypes.insert(240, QString());
  FNavitelTypes.insert(241, QString());
  FNavitelTypes.insert(242, QString());
  FNavitelTypes.insert(243, QString("service:police"));
  FNavitelTypes.insert(244, QString("health:hospital"));
  FNavitelTypes.insert(245, QString("place:townhall"));
  FNavitelTypes.insert(246, QString("service:court_of_law"));
  FNavitelTypes.insert(247, QString());
  FNavitelTypes.insert(248, QString());
  FNavitelTypes.insert(249, QString());
  FNavitelTypes.insert(250, QString());
  FNavitelTypes.insert(251, QString("sport:golf"));
  FNavitelTypes.insert(252, QString());
  FNavitelTypes.insert(253, QString("sightseeing:ruins"));
	FNavitelTypes.insert(254, QString());
	FNavitelTypes.insert(255, QString("vehicle:fuel"));
	FNavitelTypes.insert(256, QString("food:restaurant"));
	FNavitelTypes.insert(257, QString("food:bar"));
	FNavitelTypes.insert(258, QString());
  FNavitelTypes.insert(259, QString("accommodation:camping"));
  FNavitelTypes.insert(260, QString("leisure:park"));
  FNavitelTypes.insert(261, QString());
  FNavitelTypes.insert(262, QString("health:hospital"));
  FNavitelTypes.insert(263, QString("information:informationoffice"));
  FNavitelTypes.insert(264, QString("parking"));
  FNavitelTypes.insert(265, QString("service:toilets"));
  FNavitelTypes.insert(266, QString());
  FNavitelTypes.insert(267, QString("food:drinking_water"));
  FNavitelTypes.insert(268, QString("service:telephone"));
  FNavitelTypes.insert(269, QString("sightseeing:viewpoint"));
  FNavitelTypes.insert(270, QString("sport:skiing"));
  FNavitelTypes.insert(271, QString());
  FNavitelTypes.insert(272, QString());
  FNavitelTypes.insert(273, QString());
  FNavitelTypes.insert(274, QString("misc:danger"));
  FNavitelTypes.insert(275, QString());
  FNavitelTypes.insert(276, QString("aeroway"));
  FNavitelTypes.insert(277, QString("aeroway"));
  FNavitelTypes.insert(278, QString("aeroway"));
  FNavitelTypes.insert(279, QString("aeroway"));
  FNavitelTypes.insert(280, QString());
  FNavitelTypes.insert(281, QString("aeroway"));
  FNavitelTypes.insert(282, QString());
  FNavitelTypes.insert(283, QString());
  FNavitelTypes.insert(284, QString());
  FNavitelTypes.insert(285, QString());
  FNavitelTypes.insert(286, QString());
  FNavitelTypes.insert(287, QString());
  FNavitelTypes.insert(288, QString("place:house"));
  FNavitelTypes.insert(289, QString());
  FNavitelTypes.insert(290, QString());
  FNavitelTypes.insert(291, QString());
  FNavitelTypes.insert(292, QString());
  FNavitelTypes.insert(293, QString("geographic:building"));
  FNavitelTypes.insert(294, QString("place:cemetery"));
  FNavitelTypes.insert(295, QString("religion"));
  FNavitelTypes.insert(296, QString());
  FNavitelTypes.insert(297, QString());
  FNavitelTypes.insert(298, QString("waterway:dam"));
  FNavitelTypes.insert(299, QString("health:hospital"));
  FNavitelTypes.insert(300, QString("waterway:dam"));
  FNavitelTypes.insert(301, QString());
  FNavitelTypes.insert(302, QString());
  FNavitelTypes.insert(303, QString());
  FNavitelTypes.insert(304, QString());
  FNavitelTypes.insert(305, QString("leisure:park"));
  FNavitelTypes.insert(306, QString("service:post_office"));
  FNavitelTypes.insert(307, QString("education:school"));
  FNavitelTypes.insert(308, QString("geographic:tower"));
  FNavitelTypes.insert(309, QString());
  FNavitelTypes.insert(310, QString());
  FNavitelTypes.insert(311, QString("geographic:water_well"));
  FNavitelTypes.insert(312, QString());
  FNavitelTypes.insert(313, QString());
  FNavitelTypes.insert(314, QString());
  FNavitelTypes.insert(315, QString());
  FNavitelTypes.insert(316, QString());
  FNavitelTypes.insert(317, QString());
  FNavitelTypes.insert(318, QString());
  FNavitelTypes.insert(319, QString());
  FNavitelTypes.insert(320, QString());
  FNavitelTypes.insert(321, QString());
  FNavitelTypes.insert(322, QString());
  FNavitelTypes.insert(323, QString());
  FNavitelTypes.insert(324, QString());
  FNavitelTypes.insert(325, QString());
  FNavitelTypes.insert(326, QString());
  FNavitelTypes.insert(327, QString());
  FNavitelTypes.insert(328, QString());
  FNavitelTypes.insert(329, QString());
  FNavitelTypes.insert(330, QString());
  FNavitelTypes.insert(331, QString());
  FNavitelTypes.insert(332, QString());
  FNavitelTypes.insert(333, QString());
  FNavitelTypes.insert(334, QString());
  FNavitelTypes.insert(335, QString());
  FNavitelTypes.insert(336, QString("place:administrative"));
  FNavitelTypes.insert(337, QString());
  FNavitelTypes.insert(338, QString());
  FNavitelTypes.insert(339, QString());
  FNavitelTypes.insert(340, QString());
  FNavitelTypes.insert(341, QString());
  FNavitelTypes.insert(342, QString());
  FNavitelTypes.insert(343, QString());
  FNavitelTypes.insert(344, QString());
  FNavitelTypes.insert(345, QString());
  FNavitelTypes.insert(346, QString());
  FNavitelTypes.insert(347, QString());
  FNavitelTypes.insert(348, QString());
  FNavitelTypes.insert(349, QString());
  FNavitelTypes.insert(350, QString());
  FNavitelTypes.insert(351, QString());
  FNavitelTypes.insert(352, QString("leisure:nature_reserve"));
  FNavitelTypes.insert(353, QString());
  FNavitelTypes.insert(354, QString());
  FNavitelTypes.insert(355, QString());
  FNavitelTypes.insert(356, QString());
  FNavitelTypes.insert(357, QString());
  FNavitelTypes.insert(358, QString());
  FNavitelTypes.insert(359, QString());
  FNavitelTypes.insert(360, QString());
  FNavitelTypes.insert(361, QString());
  FNavitelTypes.insert(362, QString());
  FNavitelTypes.insert(363, QString());
  FNavitelTypes.insert(364, QString());
  FNavitelTypes.insert(365, QString());
  FNavitelTypes.insert(366, QString());
  FNavitelTypes.insert(367, QString());
  FNavitelTypes.insert(368, QString());
  FNavitelTypes.insert(369, QString());
  FNavitelTypes.insert(370, QString());
  FNavitelTypes.insert(371, QString());
  FNavitelTypes.insert(372, QString());
  FNavitelTypes.insert(373, QString());
  FNavitelTypes.insert(374, QString());
  FNavitelTypes.insert(375, QString());
  FNavitelTypes.insert(376, QString());
  FNavitelTypes.insert(377, QString());
  FNavitelTypes.insert(378, QString());
  FNavitelTypes.insert(380, QString());
  FNavitelTypes.insert(379, QString());
  FNavitelTypes.insert(381, QString());
  FNavitelTypes.insert(382, QString());
  FNavitelTypes.insert(383, QString("parking"));
  FNavitelTypes.insert(384, QString());
  FNavitelTypes.insert(385, QString("aeroway"));
  FNavitelTypes.insert(386, QString("place:marketplace"));
  FNavitelTypes.insert(387, QString());
  FNavitelTypes.insert(388, QString());
  FNavitelTypes.insert(389, QString("health:hospital"));
  FNavitelTypes.insert(390, QString());
  FNavitelTypes.insert(391, QString("leisure:nature_reserve"));
  FNavitelTypes.insert(392, QString());
  FNavitelTypes.insert(393, QString("geographic:building"));
  FNavitelTypes.insert(394, QString("leisure:park"));
  FNavitelTypes.insert(395, QString("leisure:park"));
  FNavitelTypes.insert(396, QString("leisure:park"));
  FNavitelTypes.insert(397, QString("leisure:park"));
  FNavitelTypes.insert(398, QString("sport:golf"));
  FNavitelTypes.insert(399, QString("sport:multi"));
  FNavitelTypes.insert(400, QString("place:cemetery"));
  FNavitelTypes.insert(401, QString("leisure:park"));
  FNavitelTypes.insert(402, QString("leisure:park"));
  FNavitelTypes.insert(403, QString("leisure:park"));
  FNavitelTypes.insert(404, QString("place:administrative"));
  FNavitelTypes.insert(405, QString("place:administrative"));
  FNavitelTypes.insert(406, QString());
  FNavitelTypes.insert(407, QString("place:administrative"));
  FNavitelTypes.insert(408, QString("place:village"));
  FNavitelTypes.insert(409, QString("service:post_office"));
  FNavitelTypes.insert(410, QString("place:townhall"));
  FNavitelTypes.insert(411, QString("place:administrative"));
  FNavitelTypes.insert(412, QString());
  FNavitelTypes.insert(413, QString("place:administrative"));
  FNavitelTypes.insert(414, QString());
  FNavitelTypes.insert(415, QString("place:administrative"));
  FNavitelTypes.insert(416, QString("place:town"));
  FNavitelTypes.insert(417, QString());
  FNavitelTypes.insert(418, QString());
  FNavitelTypes.insert(419, QString());
  FNavitelTypes.insert(420, QString());
  FNavitelTypes.insert(421, QString());
  FNavitelTypes.insert(422, QString());
  FNavitelTypes.insert(423, QString());
  FNavitelTypes.insert(424, QString("service:post_office"));
  FNavitelTypes.insert(425, QString("place:cemetery"));
  FNavitelTypes.insert(426, QString("highway:road"));
}

bool MapSearchProviderNavitel::loadFieldTypes()
{
    QDir dir(FileStorage::resourcesDirs()[0]);
    if (dir.isReadable())
        if(dir.cd(RSR_STORAGE_MAPSEARCH_NAVITEL))
            if(dir.cd(FILE_STORAGE_SHARED_DIR))
            {
                QStringList files = dir.entryList(QStringList() << FILE_STORAGE_DEFINITIONS_MASK);
                for(QStringList::const_iterator it=files.constBegin(); it!=files.constEnd(); ++it)
                {
                    QDomDocument doc;
                    QFile file(dir.filePath(*it));
                    if(file.open(QFile::ReadOnly) && doc.setContent(file.readAll(),false))
                    {
                        file.close();
                        QDomElement objElem = doc.documentElement().firstChildElement();
                        if(objElem.tagName() == "poitype")
                        {
                            for(QDomElement e = objElem; !e.isNull(); e = e.nextSiblingElement())
                            {
                                QString text = e.firstChildElement("text").text();
                                QString type = e.firstChildElement("type").text();
                                FieldType fieldType = (type == "region")?Region:
                                                      (type == "subregion")?SubRegion:
                                                      (type == "locality")?Locality:
                                                      (type == "street")?Street:
                                                                         Unknown;
                                FFieldTypes.insertMulti(fieldType, text);
                            }
                        }
                    }
                }
                return true;
            }

    return false;
}