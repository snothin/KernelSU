#ifndef __KSU_SAMSUNG_KDP_H
#define __KSU_SAMSUNG_KDP_H

#include <linux/cred.h>
#include <linux/jump_label.h>

int ksu_samsung_kdp_init(void);
void ksu_samsung_kdp_exit(void);
int ksu_samsung_kdp_commit_creds(struct cred *cred);

#ifdef CONFIG_KSU_SAMSUNG_KDP

/* Capability gate for the Samsung KDP creds suite.  Off = this kernel has
 * no KDP functions and every caller falls back to the plain cred helpers;
 * ksu_samsung_kdp_init() flips it on once, after resolving every symbol. */
extern struct static_key_false ksu_samsung_kdp_key;

void ksu_samsung_kdp_put_cred(const struct cred *cred);

static inline int ksu_commit_creds(struct cred *cred)
{
    if (static_branch_unlikely(&ksu_samsung_kdp_key))
        return ksu_samsung_kdp_commit_creds(cred);
    return commit_creds(cred);
}

static inline void ksu_put_cred(const struct cred *cred)
{
    if (static_branch_unlikely(&ksu_samsung_kdp_key))
        ksu_samsung_kdp_put_cred(cred);
    else
        put_cred(cred);
}

#else

static inline int ksu_commit_creds(struct cred *cred)
{
    return commit_creds(cred);
}

static inline void ksu_put_cred(const struct cred *cred)
{
    put_cred(cred);
}

#endif

#endif
