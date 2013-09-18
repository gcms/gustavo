select strftime('%Y-%m-%d', datetime(h.queryDate/1000, 'unixepoch')), h.src, h.dst, min(price), h.currency
from bestflights h
where queryDate < 1378954800000
group by queryDate, src, dst, currency

