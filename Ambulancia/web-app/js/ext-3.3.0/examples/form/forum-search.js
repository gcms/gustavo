/*!
 * Ext JS Library 3.3.0
 * Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com
 * http://www.extjs.com/license
 */
Ext.onReady(function() {

    var search = new Ext.form.ComboBox({
        store: new Ext.data.Store({
            proxy: new Ext.data.HttpProxy({
                url: 'http://localhost:8080/Ambulancia/viagem/listDestinos'
            }),
            reader: new Ext.data.JsonReader({
                root: 'destinos',
                totalProperty: 'totalCount',
                id: 'nome'
            }, [
                {name: 'nome', mapping: 'nome'}
            ])
        }),
        displayField:'nome',
        typeAhead: false,
        loadingText: 'Procurando...',
        width: 570,
        pageSize:10,
        hideTrigger:true,
        minChars: 3,
        applyTo: 'search',
        onSelect: function(record) { // override default onSelect to do redirect
            this.setValue(record.data.title);
            this.collapse();
        }
    });
});