from __future__ import print_function

def theoretical_PDR(LinkQuality):
    bad = 0.1

    tg = 27.5/32.5
    tb = 5/32.5

    p = 1- (tg * LinkQuality + tb * bad)

    tg = 30/32.5
    tb = 2.5/32.5

    pp = 1 - (tg * LinkQuality + tb * bad)

		# the  operator ** is exponentiation, like ^ in matlab
    q4 = pp**4
    q3 = (q4 + (1-q4)*(p**4))**2
    q2 = (q3 + (1-q3)*(p**4))**2
    q1 = (q2 + (1-q2)*(pp**2))**2

    PDR = (1-q1)
    return PDR

for link_quality in [1, 0.9, 0.8, 0.85, 0.7, 0.6, 0.5]:
	print("Link quality:", link_quality, "-> PDR:", theoretical_PDR(link_quality))

# ----  OUTPUT ----
#Link quality: 1 -> PDR: 0.999977026825
#Link quality: 0.9 -> PDR: 0.999318753719
#Link quality: 0.8 -> PDR: 0.995837698275
#Link quality: 0.85 -> PDR: 0.998137207695
#Link quality: 0.7 -> PDR: 0.985509644588
#Link quality: 0.6 -> PDR: 0.96195601785
#Link quality: 0.5 -> PDR: 0.914109202337

