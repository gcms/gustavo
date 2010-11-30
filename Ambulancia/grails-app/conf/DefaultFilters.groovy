class DefaultFilters {
    def filters = {
        all(controller:'*', action:'*') {
            before = {
                params.offset = params.offset ? params.offset.toInteger() : 0
                params.max = Math.min(params.max ? params.max.toInteger() : 10, 100)
            }
        }
    }
}