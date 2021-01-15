/**
 * @file reflow_oven_profiles.h
 *
 */

#ifndef REFLOW_OVEN_PROFILES_H
#define REFLOW_OVEN_PROFILES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    const char *title;
    const uint16_t presoak_start_temp;
    const uint16_t presoak_end_temp;
    const uint16_t presoak_dur;
    const uint16_t soak_start_temp;
    const uint16_t soak_end_temp;
    const uint16_t soak_dur;
    const uint16_t reflow_begin_start_temp;
    const uint16_t reflow_begin_end_temp;
    const uint16_t reflow_begin_dur;
    const uint16_t reflow_hold_start_temp;
    const uint16_t reflow_hold_end_temp;
    const uint16_t reflow_hold_dur;
    const uint16_t cooldown_start_temp;
    const uint16_t cooldown_end_temp;
    const uint16_t cooldown_dur;
}reflow_oven_profile_t;


const reflow_oven_profile_t reflow_profile_1 = {
    .title = "Lead-free paste 138~C [ChipQuick SMDLTLFP]",
    .presoak_start_temp = 75,
    .presoak_end_temp = 150,
    .presoak_dur = 90,
    .soak_start_temp = 150,
    .soak_end_temp = 175,
    .soak_dur = 100,
    .reflow_begin_start_temp = 175,
    .reflow_begin_end_temp = 245,
    .reflow_begin_dur = 60,
    .reflow_hold_start_temp = 245,
    .reflow_hold_end_temp = 245,
    .reflow_hold_dur = 25,
    .cooldown_start_temp = 245,
    .cooldown_end_temp = 100,
    .cooldown_dur = 60
};

const reflow_oven_profile_t reflow_profile_2 = {
    .title = "Sn63/Pb37 Optimised",
    .presoak_start_temp = 30,
    .presoak_end_temp = 150,
    .presoak_dur = 135,
    .soak_start_temp = 150,
    .soak_end_temp = 180,
    .soak_dur = 75,
    .reflow_begin_start_temp = 180,
    .reflow_begin_end_temp = 220,
    .reflow_begin_dur = 30,
    .reflow_hold_start_temp = 230,
    .reflow_hold_end_temp = 230,
    .reflow_hold_dur = 5,
    .cooldown_start_temp = 230,
    .cooldown_end_temp = 75,
    .cooldown_dur = 100
};

const reflow_oven_profile_t reflow_profile_3 = {
    .title = "Lead-free paste 138~C [ChipQuick SMDLTLFP]",
    .presoak_start_temp = 75,
    .presoak_end_temp = 150,
    .presoak_dur = 90,
    .soak_start_temp = 150,
    .soak_end_temp = 175,
    .soak_dur = 100,
    .reflow_begin_start_temp = 175,
    .reflow_begin_end_temp = 245,
    .reflow_begin_dur = 60,
    .reflow_hold_start_temp = 245,
    .reflow_hold_end_temp = 245,
    .reflow_hold_dur = 25,
    .cooldown_start_temp = 245,
    .cooldown_end_temp = 100,
    .cooldown_dur = 60
};

const reflow_oven_profile_t reflow_profile_4 = {
    .title = "Lead-free paste 138~C [ChipQuick SMDLTLFP]",
    .presoak_start_temp = 75,
    .presoak_end_temp = 150,
    .presoak_dur = 90,
    .soak_start_temp = 150,
    .soak_end_temp = 175,
    .soak_dur = 100,
    .reflow_begin_start_temp = 175,
    .reflow_begin_end_temp = 245,
    .reflow_begin_dur = 60,
    .reflow_hold_start_temp = 245,
    .reflow_hold_end_temp = 245,
    .reflow_hold_dur = 25,
    .cooldown_start_temp = 245,
    .cooldown_end_temp = 100,
    .cooldown_dur = 60
};

const reflow_oven_profile_t reflow_profile_5 = {
    .title = "Lead-free paste 138~C [ChipQuick SMDLTLFP]",
    .presoak_start_temp = 75,
    .presoak_end_temp = 150,
    .presoak_dur = 90,
    .soak_start_temp = 150,
    .soak_end_temp = 175,
    .soak_dur = 100,
    .reflow_begin_start_temp = 175,
    .reflow_begin_end_temp = 245,
    .reflow_begin_dur = 60,
    .reflow_hold_start_temp = 245,
    .reflow_hold_end_temp = 245,
    .reflow_hold_dur = 25,
    .cooldown_start_temp = 245,
    .cooldown_end_temp = 100,
    .cooldown_dur = 60
};


const reflow_oven_profile_t *const reflow_profiles[5] = {
    &reflow_profile_1,
    &reflow_profile_2,
    &reflow_profile_3,
    &reflow_profile_4,
    &reflow_profile_5,
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_PROFILES_H*/
